# Got an email from a fellow MSc student doing his thesis work 5y after I did a course for my MSc.
# He couldn't get this working and asked for help. I asked so many times for help like this during my thesis and almost never got it
# I made a promise back then I would help others if roles were reversed. So even if it's saturday evening: here is a POC
# Yes, the code still works if you just use some old version 3.20
# This is a POC, i.e., Docker best practices are omitted
FROM ubuntu:14.04
RUN apt-get update; apt-get install -y build-essential gcc python git wget
RUN mkdir /build; cd /build; wget http://code.nsnam.org/ns-3.20/archive/tip.tar.gz; tar -zxvf tip.tar.gz; git clone https://github.com/hansbogert/TUD-IN4398-Project2.git
RUN cd /build/ns-3-*/src; ln -s ../../TUD-IN4398-Project2/ns3/src/iot
RUN cd /build/ns-3-*; ./waf configure --enable-tests
RUN cd /build/ns-3-*; ./waf -j8 build
RUN cd /build/ns-3-*; ./test.py -s iot
