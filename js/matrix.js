// 
// Produce the cartesian product of multiple vectors of different lengths, in the
// form of an array of arrays, where each array is the length of the number of inputs.
//
// https://en.wikipedia.org/wiki/Cartesian_product
//
function combine() {
    var paths = [];
    var f = (path, args) => {
        if (args.length == 0) {
            paths.push(path);
        } else if (args[0].length == 0) {
            paths.push(path);
        } else {
            for (const v of args[0]) {
                var p = path.concat(v);
                f(p, args.slice(1));
            }
        }
    }
    
    f([], Object.values(arguments));
    return paths;
}

//
// Given a set of input vectors of any length, return the combination at
// $index, without having to pre-calculate the cartesian product of the vectors.
//
function reverseLookup(sources, index) {
    var indexes = [];
    var values = [];

    var _i = index;

    for (var i = sources.length - 1; i >= 0; i--) {
        var source = sources[i];
        var sourceIndex = _i % source.length;
        _i = Math.floor(_i / source.length);
        indexes[i] = sourceIndex;
        values[i] = source[sourceIndex];
    }

    return values;
}


var a = ["a", "b", "c", "d"];
var b = ["aa", "bb", "cc", "dd", "ee"];
var c = ["aaa", "bbb", "ccc", "ddd", "eee", "fff", "ggg"];
var d = ["hhhhhh"];
var e = ["aa", "bb", "cc", "dd", "ee"];

var results = combine(a, b, c, d, e);
console.log("Expected combinations:", a.length * b.length * c.length * d.length * e.length);
console.log("Cartesian Product Length:", results.length);

//
// Throw an exception if there's a mismatch between the generated product and the 
// value produced by the reverse lookup.
//

var isEqual = (a, b) => {
    return a.length == b.length && a.every((v, i) => { return v = b[i]});
};

var sources = [a, b, c, d, e];
for (var i = 0; i < results.length; ++i) {
    var r0 = results[i];
    var r1 = reverseLookup(sources, i);
    var r2 = reverseLookupHACK(i);

    if (!isEqual(r0, r1) && !isEqual(r0, r2) && !isEqual(r1, r2)) {
        throw new Error("Wrong!")
    }
}

//
// Just a hack using the specific arrays define above (a,b,c,...), to check the 
// correctness of the previous functions.
//
function reverseLookupHACK(index) {
    var ee = index % e.length;
    var _i = Math.floor(index / e.length);
    var dd = _i % d.length;
    _i = Math.floor(_i / d.length);
    var cc = _i % c.length;
    _i = Math.floor(_i / c.length);
    var bb = _i % b.length;
    _i = Math.floor(_i / b.length);
    var aa = _i;

    return [a[aa], b[bb], c[cc], d[dd], e[ee]];
}

console.log("Cartesian product entry at index 351\t", results[351]);
console.log("Entry at reverse lookup index 351\t", reverseLookup([a, b, c, d, e], 351));
console.log("Entry at reverse lookup hack 351\t", reverseLookupHACK(351));