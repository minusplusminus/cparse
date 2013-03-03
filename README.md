CParse
======
A C library to use the REST API at (parse.com)[http://parse.com].

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
CParseArray *array = cparse_array_new();

cparse_array_add_number(array, 1234);

CParseObject *obj = cparse_object_new();

cparse_object_set_array(obj, "numbers", array);

char buf[BUFSIZ];

cparse_object_to_json(obj, buf, 0);

puts(buf); 

/* will print:
{"numbers":[1234]}
*/
```