```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

const int MAX_N = 100005;
const int MAX_VAL = 100;

int n, m, q;
int arr[MAX_N];
int freq[MAX_VAL + 1];
int blockSize;

struct Query {
    int l, r, idx;
    bool operator<(const Query& other) const {
        int block_a = l / blockSize;
        int block_b = other.l / blockSize;
        if (block_a != block_b)
            return block_a < block_b;
        return r < other.r;
    }
};

std::vector<Query> queries;
int answers[MAX_N];

void add(int pos) {
    freq[arr[pos]]++;
}

void remove(int pos) {
    freq[arr[pos]]--;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> n >> m >> q;
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    blockSize = static_cast<int>(sqrt(n));

    queries.resize(q);
    for (int i = 0; i < q; ++i) {
        int l, r;
        std::cin >> l >> r;
        queries[i] = {l - 1, r - 1, i};
    }

    std::sort(queries.begin(), queries.end());

    int currL = 0, currR = -1;
    for (const auto& query : queries) {
        while (currR < query.r) {
            add(++currR);
        }
        while (currR > query.r) {
            remove(currR--);
        }
        while (currL < query.l) {
            remove(currL++);
        }
        while (currL > query.l) {
            add(--currL);
        }

        int maxFreq = 0;
        int maxVal = -1;
        for (int i = 1; i <= m; ++i) {
            if (freq[i] > maxFreq) {
                maxFreq = freq[i];
                maxVal = i;
            }
        }
        answers[query.idx] = maxVal;
    }

    for (int i = 0; i < q; ++i) {
        std::cout << answers[i] << "\n";
    }

    return 0;
}
```
