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

obj.set_str("name", "Harry Potter");

obj.set_int("age", 24);

obj.set_double("money", 102.34);

PFArray array;

array.add_int(1234);

array.add_str("4567");

obj.set_array("numbers", array);

if(obj.save())
	cout << "Success!" << endl;

/* Voila, we have saved an Example object */

```

<!--
Background Operations
=====================
```
void my_nifty_callback(CParseObject *obj, CParseError *error)
{
	if(error) {
		log(error);
		return;
	}

	do_some_other_action_with_object(obj);
}

CParseObject *obj = cparse_object_new();

cparse_object_set_string(obj, "main", "Hello, World");

cparse_object_save_in_background(obj, my_nifty_callback);

```
-->