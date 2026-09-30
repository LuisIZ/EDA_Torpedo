#include <bits/stdc++.h>
using namespace std;

const long long INF = LLONG_MAX;

struct FractionalCascading {
    int k;
    vector<vector<long long>> aumentada, respuesta;
    vector<vector<int>> puente;

    FractionalCascading(vector<vector<long long>> listas) {
        k = listas.size();
        aumentada.resize(k);
        respuesta.resize(k);
        puente.resize(k);
        for (int i = k - 1; i >= 0; i--) {
            sort(listas[i].begin(), listas[i].end());
            vector<long long> muestra;
            if (i + 1 < k) {
                auto &sig = aumentada[i + 1];
                for (int j = 1; j + 1 < (int)sig.size(); j += 2) muestra.push_back(sig[j]);
            }
            auto &M = aumentada[i];
            merge(listas[i].begin(), listas[i].end(), muestra.begin(), muestra.end(), back_inserter(M));
            M.push_back(INF);
            int sz = M.size();
            respuesta[i].assign(sz, INF);
            int p = 0;
            for (int j = 0; j < sz; j++) {
                while (p < (int)listas[i].size() && listas[i][p] < M[j]) p++;
                respuesta[i][j] = p < (int)listas[i].size() ? listas[i][p] : INF;
            }
            if (i + 1 < k) {
                auto &sig = aumentada[i + 1];
                puente[i].assign(sz, 0);
                int q = 0;
                for (int j = 0; j < sz; j++) {
                    while (sig[q] < M[j]) q++;
                    puente[i][j] = q;
                }
            }
        }
    }

    vector<long long> sucesores(long long x) {
        vector<long long> res(k);
        if (!k) return res;
        int pos = lower_bound(aumentada[0].begin(), aumentada[0].end(), x) - aumentada[0].begin();
        for (int i = 0; i < k; i++) {
            res[i] = respuesta[i][pos];
            if (i + 1 < k) {
                pos = puente[i][pos];
                if (pos > 0 && aumentada[i + 1][pos - 1] >= x) pos--;
            }
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int k;
    cin >> k;
    vector<vector<long long>> listas(k);
    for (int i = 0; i < k; i++) {
        int sz;
        cin >> sz;
        listas[i].resize(sz);
        for (auto &x : listas[i]) cin >> x;
    }
    FractionalCascading fc(listas);
    int q;
    cin >> q;
    while (q--) {
        long long x;
        cin >> x;
        vector<long long> res = fc.sucesores(x);
        for (int i = 0; i < k; i++) {
            if (i) cout << ' ';
            if (res[i] == INF) cout << -1;
            else cout << res[i];
        }
        cout << '\n';
    }
    return 0;
}
