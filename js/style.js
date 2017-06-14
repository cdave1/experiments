//
// Guard function styles in javascript.
//
'use strict';

function onError(message) {
    console.error(message);
}

function guardFunctionA(input) {
    if (input) {
        if (input.firstVariable) {
            console.log("The first variable was:", input.firstVariable);
            return;
        }

        onError("missing first variable");
    } else {
        onError("missing input");
    }
}

function guardFunctionB(input) {
    if (!input) {
        onError("missing input");
        return;
    }

    if (!input.firstVariable) {
        onError("missing first variable");
        return;
    }

    console.log("The first variable was:", input.firstVariable);
}

function guardFunctionC(input) {
    if (input) {
        if (input.firstVariable) {
            if (input.firstVariable.whatever) {
                console.log(input.firstVariable.whatever);
            } else {
                onError("Missing!")
            }
        } else {
            onError("missing first variable");
        }
    } else {
        onError("missing input");
    }
}

function checkVariables(input) {
    return input.firstVariable && input.firstVariable.whatever && input.secondVariable && true;
}

function guardFunctionD(input) {
    //var firstVariable = input.firstVariable && input.firstVariable.whatever ? input.firstVariable.whatever : null;
    var firstVariable = _.get(input, "firstVariable.whatever", null);

    // THREE PROBLEMS:
    // 1. You now have another layer of indirection.
    // 2. What if checkVariables changes?
    // 3. You can't tell that a variable is missing, only that any of the expected variables are missing.
    if (checkVariables(input)) {
        onError("Missing")
    }

    var expected = ['firstVariable.whatever', 'secondVariable'];

    if (firstVariable == null) {
        onError("Missing!")
    } else {
        console.log("The first variable was:", firstVariable);
    }
}

function guardFunctionE(input) {
    var first = _.get(input, "first", null);
    var second = _.get(input, "second", null);

    if (firstVariable == null) {
        onError("Missing!")
    } else if (second == null) {
        onError("Missing!")
    } else {
        console.log("The first variable was:", firstVariable);
    }
}

function similarFunctionA(input) {
    var first = _.get(input, "first", null);
    var second = _.get(input, "second", null);
    var third = _.get(input, "third", null);

    if (first == null) {
        onError("Missing first variable!");
    } else if (second == null) {
        onError("Missing second variable!");
    } else if (third == null) {
        onError("Missing third variable!");
    } else {
        console.log("The variables were:", first, second);
    }
}

function similarFunctionB(input) {
    var first = _.get(input, "first", null);
    var second = _.get(input, "second", null);
    var third = _.get(input, "third", null);

    if (first == null) {
        onError("Missing first variable!");
    } else if (second == null) {
        onError("Missing second variable!");
    } else {
        console.log("The variables were:", first, second);
    }
}

function filter(input, next) {
    var first = _.get(input, "first", null);
    var second = _.get(input, "second", null);
    var third = _.get(input, "third", null);

    if (first == null) {
        onError("Missing first variable!");
    } else if (second == null) {
        onError("Missing second variable!");
    } else {
        next(input);
    }
}

var good = {
    first: "first",
    second: "second",
    third: "third"
};

var bad = {
    first: "first",
    second: "second"
};

function doStuff(input) {
    console.log("No errors here!", input.first, input.second, input.third);
    console.log(input.fourth);
}

filter(good, printStuff);
filter(bad, printStuff);