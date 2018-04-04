FROM       icecubetray/alpine-icy
MAINTAINER icecubetray

COPY sbin/run.sh /usr/sbin/run_serum_cst
RUN dos2unix -u /usr/sbin/run_serum_cst

CMD [ "/usr/sbin/run_serum_cst" ]
