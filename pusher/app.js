var spawn = require('child_process').spawn;
var fs = require('mz/fs')
var Inotify = require('inotify').Inotify;
var inotify = new Inotify();
var sensorDataPath = '/sensorsdata/';

inotify.addWatch({
    path: sensorDataPath,
    watch_for: Inotify.IN_ALL_EVENTS,
    callback: onNewFileGenerated
});
startExtractorProcess();
console.log("running camaera sensors")
return;

function onNewFileGenerated(event) {
    var mask = event.mask;
    if (mask & Inotify.IN_CLOSE_WRITE) {
        var fileName = event.name;
        handleReadingFileGeneratedV2(fileName);
    }
}

async function handleReadingFileGeneratedV2(fileName) {
    var filePath = sensorDataPath + fileName;
    var data = await fs.readFile(filePath, 'utf8');
    var content = { data: data, fileName: fileName};
    console.log(JSON.stringify(content));
    await fs.unlink(filePath);
}

function startExtractorProcess () {
    var extractorProcess = spawn('/code/extractor/RFSniffer'
        , [
            '/sensorsdata/'
        ]);
    extractorProcess.stderr.on('data', (data) => {
        console.error(`child stderr:\n${data.toString()}`);
    });
}









// Catch uncaught exception
process.on('uncaughtException', err => {
    console.dir(err, { depth: null });
    process.exit(1);
});
process.on('exit', code => {
    console.log('Process exit');
    process.exit(code);
});
process.on('SIGTERM', code => {
    console.log('Process SIGTERM');
    process.exit(code);
});