#include <bits/stdc++.h>
using namespace std;

struct LayeredRangeTree {
    int n;
    vector<int> xs, ids;
    vector<vector<pair<int, int>>> lista;
    vector<vector<int>> haciaIzq, haciaDer;

    LayeredRangeTree(vector<array<int, 3>> puntos) {
        n = puntos.size();
        sort(puntos.begin(), puntos.end());
        for (auto &p : puntos) xs.push_back(p[0]);
        lista.resize(4 * max(n, 1));
        haciaIzq.resize(4 * max(n, 1));
        haciaDer.resize(4 * max(n, 1));
        if (n) construir(1, 0, n - 1, puntos);
    }

    void construir(int nodo, int l, int r, vector<array<int, 3>> &puntos) {
        if (l == r) {
            lista[nodo].push_back({puntos[l][1], puntos[l][2]});
            return;
        }
        int m = (l + r) / 2;
        construir(2 * nodo, l, m, puntos);
        construir(2 * nodo + 1, m + 1, r, puntos);
        auto &A = lista[2 * nodo], &B = lista[2 * nodo + 1];
        auto &C = lista[nodo];
        merge(A.begin(), A.end(), B.begin(), B.end(), back_inserter(C));
        int sz = C.size();
        haciaIzq[nodo].assign(sz + 1, 0);
        haciaDer[nodo].assign(sz + 1, 0);
        int i = 0, j = 0;
        for (int k = 0; k < sz; k++) {
            while (i < (int)A.size() && A[i].first < C[k].first) i++;
            while (j < (int)B.size() && B[j].first < C[k].first) j++;
            haciaIzq[nodo][k] = i;
            haciaDer[nodo][k] = j;
        }
        haciaIzq[nodo][sz] = A.size();
        haciaDer[nodo][sz] = B.size();
    }

    int contarEn(int nodo, int l, int r, int ql, int qr, int a, int b) {
        if (qr < l || r < ql || a >= b) return 0;
        if (ql <= l && r <= qr) return b - a;
        int m = (l + r) / 2;
        return contarEn(2 * nodo, l, m, ql, qr, haciaIzq[nodo][a], haciaIzq[nodo][b]) +
               contarEn(2 * nodo + 1, m + 1, r, ql, qr, haciaDer[nodo][a], haciaDer[nodo][b]);
    }

    void reportarEn(int nodo, int l, int r, int ql, int qr, int a, int b, vector<int> &res) {
        if (qr < l || r < ql || a >= b) return;
        if (ql <= l && r <= qr) {
            for (int k = a; k < b; k++) res.push_back(lista[nodo][k].second);
            return;
        }
        int m = (l + r) / 2;
        reportarEn(2 * nodo, l, m, ql, qr, haciaIzq[nodo][a], haciaIzq[nodo][b], res);
        reportarEn(2 * nodo + 1, m + 1, r, ql, qr, haciaDer[nodo][a], haciaDer[nodo][b], res);
    }

    bool prepararConsulta(int x1, int x2, int y1, int y2, int &ql, int &qr, int &a, int &b) {
        if (!n || x1 > x2 || y1 > y2) return false;
        ql = lower_bound(xs.begin(), xs.end(), x1) - xs.begin();
        qr = upper_bound(xs.begin(), xs.end(), x2) - xs.begin() - 1;
        if (ql > qr) return false;
        auto &raiz = lista[1];
        a = lower_bound(raiz.begin(), raiz.end(), make_pair(y1, INT_MIN)) - raiz.begin();
        b = upper_bound(raiz.begin(), raiz.end(), make_pair(y2, INT_MAX)) - raiz.begin();
        return true;
    }

    int contar(int x1, int x2, int y1, int y2) {
        int ql, qr, a, b;
        if (!prepararConsulta(x1, x2, y1, y2, ql, qr, a, b)) return 0;
        return contarEn(1, 0, n - 1, ql, qr, a, b);
    }

    vector<int> reportar(int x1, int x2, int y1, int y2) {
        vector<int> res;
        int ql, qr, a, b;
        if (prepararConsulta(x1, x2, y1, y2, ql, qr, a, b)) reportarEn(1, 0, n - 1, ql, qr, a, b, res);
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<array<int, 3>> puntos(n);
    for (int i = 0; i < n; i++) {
        cin >> puntos[i][0] >> puntos[i][1];
        puntos[i][2] = i + 1;
    }
    LayeredRangeTree lrt(puntos);
    int q;
    cin >> q;
    while (q--) {
        int tipo, x1, x2, y1, y2;
        cin >> tipo >> x1 >> x2 >> y1 >> y2;
        if (tipo == 1) {
            cout << lrt.contar(x1, x2, y1, y2) << '\n';
        } else {
            vector<int> res = lrt.reportar(x1, x2, y1, y2);
            cout << res.size();
            for (int id : res) cout << ' ' << id;
            cout << '\n';
        }
    }
    return 0;
}
