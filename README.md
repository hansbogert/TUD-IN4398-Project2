# TUD-IN4398-Project2
NOTE: Code no longer compiles with NS-3 >3.20


This is the source code for the IoT (Internet of things) Seminar
@TUDelft, course: IN4398

After pulling the source code can then be symlinked to your NS3 folder:

````
cd $NS3_ROOT/src
ln -s $PULL_DIR/ns3/src/iot
cd $NS3_ROOT/
ln -s $PULL_DIR/ns3/scratch
````

Now you must reconfigure NS3 to incorporate the module:

````
./waf configure $YOUR_OPTIONS
````

Afterwards it the specific 'iot' module can be tested by issuing:
````
./test.py -s iot
````

## Document
The project document `Project.asciidoc` can be built by issuing:

````
asciidoc -a latexmath Project.asciidoc
````

Mind that you need `graphviz` for filtering/compiling dot-formatted
graphs.
