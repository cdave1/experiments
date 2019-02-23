function conflict(a, b) {
    return !(b[0] <= b[1] && (b[1] <= a[0] || b[0] >= a[1]));
}

var results = [
    conflict([0, 1], [2, 5]), // no
    conflict([6, 8], [2, 5]), // no
    conflict([1, 5], [2, 5]), // YES
    conflict([3, 4], [2, 5]), // YES
    conflict([1, 8], [2, 5]), // YES
    conflict([1, 2], [2, 5])  // no
];

console.log(results);