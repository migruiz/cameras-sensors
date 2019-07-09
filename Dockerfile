FROM balenalib/raspberrypi3-node:8-latest
RUN [ "cross-build-start" ]



RUN apt-get update && \
apt-get install -yqq --no-install-recommends curl g++ gcc make  supervisor && rm -rf /var/lib/apt/lists/*



RUN curl -o wiringpi.tar.gz  "https://git.drogon.net/?p=wiringPi;a=snapshot;h=8d188fa0e00bb8c6ff6eddd07bf92857e9bd533a;sf=tgz" \
&&  mkdir /wiringPi \
&& tar -xzf wiringpi.tar.gz  -C /wiringPi --strip-components=1 \
&& cd /wiringPi/ \
&& ./build \
&& cd ..

RUN mkdir /code/

RUN mkdir /code/extractor/ 
COPY extractor /code/extractor

RUN cd /code/extractor \
&& make 

RUN  mkdir /code/pusher/ 

COPY pusher/package.json  /code/pusher/package.json

RUN cd /code/pusher \
&& npm  install 


COPY pusher /code/pusher



RUN [ "cross-build-end" ]  





ENTRYPOINT ["node","/code/pusher/app.js"]


