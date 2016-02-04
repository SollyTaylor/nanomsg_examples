# nanomsg_examples
The Qt based nanomsg examples in C++11

## build instructions
1. The fundamental nanomsg libs according to the official [nanomsg repository](https://github.com/nanomsg/nanomsg), follow the build instructions 
	1. On Mac/Linux, set the **configure** prefix as the **build** directory in current nanomsg_examples repository.
	2. On Windows, set the **CMAKE_CMAKE_INSTALL_PREFIX** as the **build** directory in current nanomsg_examples repository.
	so that the build Qt pro file could find the target nanomsg includes and binary library
2. I copied directly the source files of C++11 bindings of original C nanomsg by means of [nanomsgxx](https://github.com/achille-roussel/nanomsgxx), since I am not familiar with the python-based building system **waf**. It is natural for the original nanomsgxx repository to vary its versions.

The most benefitial things of this nanomsg_examples project are:

1. Demonstrations of usages of **tcp** protocol usage rather than the examples prevail in different source showing the examples with only **inter-process-communications** (or said as **inproc**, **ipc**)
2. Using the **rep/req** pattern to secure the **server/client** package transmissions, three classical patterns known namely as **rep/req**, **pub/sub** and **pipeline** in the **zeromq** are all shown in the project.

Modify the following code
	```
	int main(int argc, char* argv[])
	{
	    int working_mode = REPREQ;
	//    int working_mode = PUBSUB;
	//    int working_mode = PIPELINE;
	...
	```
	to alternate the demonstration patterns.

The license is MIT.
