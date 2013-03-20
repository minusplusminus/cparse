CParse
======
A C++ library to use the REST API at [parse.com](http://parse.com).


Setup
=====
- Not required but [Homebrew](http://mxcl.github.com/homebrew/) is pretty handy to install other required libs
- The build system uses [Premake](http://industriousone.com/premake) for cross platform compiling (OSX: brew install premake)
- run 'premake4 gmake' or 'premake4 vs2010' depending on your environment, which will build the makefile or project file
- run 'make' to compile the library and run the unit tests

Dependencies
============

- libcurl for HTTP requests (should be already installed on OSX, otherwise 'brew install curl')
- libjson for JSON parsing ('brew install json')
- [igloo](http://igloo-testing.org) for unit testing

Example
=======
```
PFObject obj("Example");

// add a string
obj.set_str("name", "Harry Potter");

// add an int
obj.set_int("age", 24);

// add a double
obj.set_double("money", 102.34);

// add an array
PFArray array;
array.add_int(1234);
array.add_str("4567");

obj.set_array("numbers", array);

if(obj.save())
	cout << "Success!" << endl;

/* Voila, we have saved an Example object on Parse.com */

if(obj.destroy())
	cout <<  "Deleted!" << endl;

```

More Examples
=============

```
// setup your app info
Parse::set_application_id("AuIDhfjSJdiIFIFKDSJSJSY6KDKD8838");
Parse::set_api_key("LdfhJjdudJDjDJJMmUfkfjjhd7d7ld8484mJJJ");

std::thread thread = someObj->saveInBackground([&](PFObject *obj) {
	cout << "Object Saved was " << obj->id() << endl;
});

thread.join();

