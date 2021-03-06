# XML_Element_Parser

A tool to query and list XML nodes of a user defined element name to a listbox. Any depth of parent/child relationships will be searched through. This is useful when you need to query for specific nodes in an XML that is very long. 

## Features
* Print out nodes of user defined element name
* Print all nodes 
* Copy entry from listbox to clipboard

## Example
![XML_Element_Parser](https://user-images.githubusercontent.com/7757495/149802020-5a5eee40-cbf3-4dbe-b548-db34d3db30a6.png)

## Windows building
1. Requires [wxWidgets](https://www.wxwidgets.org/downloads/) GUI library. You will probably need to download and build the library from source. I used wxWidgets-3.1.5 for XML_Element_Parser.

2. Make sure the compiler knows where to look for the header file(s) for the library. Open the .sln and navigate to **Project > Properties > Configuration Properties > C/C++ > General**. Then under **Additional Include Directories** add the location for the header files:
    -  `...\wxWidgets-3.1.5\include`
    -  `...\wxWidgets-3.1.5\include\msvc`

3. Tell the linker where to look for the library file(s). Open the .sln and navigate to **Project > Properties > Configuration Properties > Linker > General**. Then under **Additional Library Directories** add path for library files. 
    - `...\wxWidgets-3.1.5\lib\vc_lib`
