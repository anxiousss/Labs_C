const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

let n;

function bfsTreeCreating(matrix, start) {
    let d = new Array(n).fill(false);
    let q = [start];
    d[start] = true;
    while (q.length > 0) {
        let u = q.shift();
        for (let i = 0; i < n; ++i) {
            if (matrix[u][i] === 1 && !d[i]) {
                q.push(i);
                matrix[u][i] = 2;
                d[i] = true;
            }
        }
    }

    let bfs_tree = Array.from(Array(n), () => new Array(n).fill(0));
    for (let i = 0; i < n; ++i) {
        for (let j = 0; j < n; ++j) {
            if (matrix[i][j] === 2) {
                bfs_tree[i][j] = 1;
                bfs_tree[j][i] = 1;
            }
        }
    }
    return bfs_tree;
}
function dVertex(matrix, start) {
    let q = [start];
    let d = new Array(n).fill(-1);
    d[start] = 0;
    while (q.length > 0) {
        let u = q.shift();
        for (let i = 0; i < n; ++i) {
            if (matrix[u][i] === 1 && d[i] === -1) {
                q.push(i);
                d[i] = d[u] + 1;
            }
        }
    }
    return d;
}

function findComponents(matrix) {
    let color = 0;
    let d = new Array(n).fill(0);
    for (let i = 0; i < n; ++i) {
        if (!d[i]) {
            let q = [i];
            ++color;
            d[i] = color;
            while (q.length > 0) {
                let u = q.shift();
                for (let j = 0; j < n; ++j) {
                    if (matrix[u][j] === 1 && !d[j]) {
                        q.push(j);
                        d[j] = color;
                    }
                }
            }
        }
    }
    return d;
}

function promptUser(question) {
    return new Promise((resolve) => {
        rl.question(question, (answer) => {
            resolve(answer);
        });
    });
}

async function readMatrix() {
    n = parseInt(await promptUser("Введите количество вершин: "));
    let matrix = Array.from(Array(n), () => new Array(n).fill(0));

    // Ожидаем ввод всей матрицы в одной строке, разделенной пробелами
    const matrixInput = (await promptUser(`Введите элементы матрицы через пробелы: `)).trim();
    const matrixValues = matrixInput.split(/\s+/).map(Number);

    // Проверяем, что введено достаточное количество элементов
    if (matrixValues.length !== n * n) {
        console.log(`Ожидалось ввести ${n * n} элементов, но получено ${matrixValues.length}.`);
        rl.close();
        return;
    }

    // Заполняем матрицу значениями из введенной строки
    for (let i = 0; i < n; ++i) {
        for (let j = 0; j < n; ++j) {
            matrix[i][j] = matrixValues[i * n + j];
        }
    }

    return matrix;
}
async function main() {
    const matrix = await readMatrix();

    // поиск путей из вершины
    let vertex = dVertex(matrix, 0);
    console.log(vertex.join(' '));

    // поиск компонент связности
    let components = findComponents(matrix);
    console.log(components.join(' '));

    // создание bfs дерева
    let tree = bfsTreeCreating(matrix, 0);
    for (let i = 0; i < n; ++i) {
        console.log(tree[i].join(' '));
    }

    rl.close();
}

main();
