FROM balenalib/raspberrypi3-alpine-node:8-latest
RUN [ "cross-build-start" ]


#4.1.3
RUN apk add --update curl xz make python g++ gcc supervisor sudo \
&& curl -o wiringpi.tar.gz  "https://git.drogon.net/?p=wiringPi;a=snapshot;h=8d188fa0e00bb8c6ff6eddd07bf92857e9bd533a;sf=tgz" \
&&  mkdir /wiringPi \
&& tar -xzf wiringpi.tar.gz  -C /wiringPi --strip-components=1 \
&& cd /wiringPi/ \
&& ./build \
&& cd .. \
&& apk del curl xz  \
&& rm -rf wiringpi.tar.gz  \
&& rm -rf /var/cache/apk/*



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


COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

RUN [ "cross-build-end" ]  





ENTRYPOINT ["/usr/bin/supervisord"]


