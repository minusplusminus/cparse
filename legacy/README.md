CParse
======
A C library to use the REST API at [parse.com](http://parse.com).


Setup
=====
- Not required but [Homebrew](http://mxcl.github.com/homebrew/) is pretty handy for install libs
- The build system uses [Premake](http://industriousone.com/premake) for cross platform compiling (OSX: brew install premake)
- run 'premake4 gmake' or 'premake4 vs2010' depending on your environment, which will build the makefile or project file
- run 'make' to compile the library and run the unit tests

Code style
==========
- function names: lower case underscored, prefixed with cparse namespace
- typedefs and defines: upper case underscored, prefixed with CPARSE namespace
- enums and variables: camel case

Dependencies
============

- libcurl for HTTP requests (should be already installed on OSX, otherwise 'brew install curl')
- libjson for JSON parsing ('brew install json')
- libcheck for unit testing ('brew install check')

Example
=======
```
CParseObject *obj = cparse_object_with_class_name("Example");

cparse_object_set_string(obj, "name", "Harry Potter");

cparse_object_set_number(obj, "age", 24);

cparse_object_set_real(obj, "money", 102.34);

cparse_object_save(obj);

cparse_object_free(obj);

/* Voila, we have saved an Example object */

```

Another Example
===============
```
CParseJSONArray *array = cparse_array_new();

cparse_array_add_number(array, 1234);

cparse_array_add_string(array, "4567");

CParseObject *obj = cparse_object_new();

cparse_object_set_array(obj, "numbers", array);

char buf[BUFSIZ];

cparse_object_to_json(obj, buf, 0);

puts(buf); 

cparse_object_free(obj);

/* will print:
{"numbers":[1234,"4567"]}
*/
```

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