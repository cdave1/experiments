'use strict';

const EMPTY = -999;
const NOUGHT = 1;
const CROSS = -1;

const SIZE = 3;

function sum(s) {
    var t = 0;
    s.forEach(v => {
        t += v;
    });
    return t;
}


function getScore(gameSpace) {
    // Horizontal
    gameSpace.forEach(row => {
        var total = sum(row);
        if (Math.abs(total) == SIZE) {
            return total;
        }
    });

    // Vertical
    for (var column = 0; column < gameSpace[0].length; ++column) {
        var total = sum([gameSpace[0][column], gameSpace[1][column], gameSpace[2][column]]);
        if (Math.abs(total) == SIZE) {
            return total;
        }
    }

    // Diagonal
    var t1 = sum([gameSpace[0][0], gameSpace[1][1], gameSpace[2][2]]);
    var t2 = sum([gameSpace[2][0], gameSpace[1][1], gameSpace[0][2]]);

    if (Math.abs(t1) == SIZE) { 
        return t1;
    } else if (Math.abs(t2) == SIZE) {
        return t2;
    }

    return 0;
}


function printGameSpace(gameSpace) {
    var s = "";
    for (var row = 0; row < SIZE; row++) {
        for (var col = 0; col < SIZE; col++) {
            var p = gameSpace[row][col];
            if (p == EMPTY) {
                s = s.concat('_');
            } else if (p == NOUGHT) {
                s = s.concat('O');
            } else {
                s = s.concat('X');
            }
        }
        s = s.concat('\n');
    }
    console.log(s);
}


function addMove(game, player, loc) {
    var next = [];
    for (var row = 0; row < SIZE; row++) {
        var _r = [];
        for (var col = 0; col < SIZE; col++) {
            _r[col] = game[row][col];
        }
        next.push(_r);
    }
    if (next[loc.row][loc.col] == EMPTY) {
        next[loc.row][loc.col] = player;
        return next;
    } else {
        return null;
    }
}


function gameSpace() {
    var s = [];
    for (var row = 0; row < SIZE; row++) {
        var _r = [];
        for (var col = 0; col < SIZE; col++) {
            _r[col] = EMPTY;
        }
        s.push(_r);
    }
    return s;
}


function isFull(gameSpace) {
    for (var row = 0; row < SIZE; row++) {
        for (var col = 0; col < SIZE; col++) {
            if (gameSpace[row][col] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}


function run(players) {
    var step = 0;
    var currentGameSpace = gameSpace();

    while (true) {
        var currentPlayer = players[step % 2];

        var moves = [];
        for (var row = 0; row < SIZE; row++) {
            for (var col = 0; col < SIZE; col++) {
                if (Math.random() > 0.5) {
                    moves.push({row: row, col: col});
                } else {
                    moves.unshift({row: row, col: col});
                }
            }
        }

        for (const move of moves) {
            var nextGameSpace = addMove(currentGameSpace, currentPlayer, move);
            if (nextGameSpace) {
                currentGameSpace = nextGameSpace;
                break;
            }
        }

        step++;

        var score = getScore(currentGameSpace);
        if (score == SIZE * currentPlayer) {
            console.log("WIN", currentPlayer);
            printGameSpace(currentGameSpace);
            return;
        } else if (isFull(currentGameSpace)) {
            console.log("NO WINNER FOUND", currentPlayer);
            printGameSpace(currentGameSpace);
            break;
        }
    }
}

run([NOUGHT, CROSS]);