
var a = ['a', 'b', 'c', 'd', 'e'];

// This is fine...
for (const c of a) {
    console.log(c);
}

// .. but can't do this:
for (const c, i of a) {
    console.log(c, i);
}