
#!/bin/bash
g++ -std=c++11 -o roughcosmics roughcosmics.C -lstdc++ `root-config --libs` -I$ROOTSYS/include

