var spawn = require('child_process').spawn;
var fs = require('mz/fs')
var Inotify = require('inotify').Inotify;
var inotify = new Inotify();
var mqtt = require('./mqttCluster.js');
var sensorDataPath = '/sensorsdata/';
global.sensorReadingTopic = 'sensorReadingTopic';
global.mtqqLocalPath = process.env.MQTTLOCAL;
inotify.addWatch({
    path: sensorDataPath,
    watch_for: Inotify.IN_ALL_EVENTS,
    callback: onNewFileGenerated
});
startExtractorProcess();
console.log("running camaera sensors")
return;

async function onNewFileGenerated(event) {
    var mask = event.mask;
    if (mask & Inotify.IN_CLOSE_WRITE) {
        var fileName = event.name;
        await handleReadingFileGeneratedV2(fileName);
    }
}

async function handleReadingFileGeneratedV2(fileName) {
    var filePath = sensorDataPath + fileName;
    var data = await fs.readFile(filePath, 'utf8');
    var content = { data: data, fileName: fileName};
    var mqttCluster = await mqtt.getClusterAsync(); 
    mqttCluster.publishData(global.sensorReadingTopic, content);
    await fs.unlink(filePath);
}

function startExtractorProcess () {
    var extractorProcess = spawn('/code/extractor/RPi_utils/RFSniffer'
        , [
            '/sensorsdata/'
        ]);
        extractorProcess.stdout.on('data', (data) => {
            console.log(data.toString());
        });
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