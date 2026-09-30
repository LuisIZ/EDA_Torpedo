#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    vector<int> clave, cnt, tam, padre;
    vector<array<int, 2>> hijo;
    int raiz = 0;
    long long rotaciones = 0;

    SplayTree() {
        clave.push_back(0);
        cnt.push_back(0);
        tam.push_back(0);
        padre.push_back(0);
        hijo.push_back({0, 0});
    }

    int nuevoNodo(int x, int p) {
        clave.push_back(x);
        cnt.push_back(1);
        tam.push_back(1);
        padre.push_back(p);
        hijo.push_back({0, 0});
        return (int)clave.size() - 1;
    }

    void actualizar(int u) {
        if (u) tam[u] = tam[hijo[u][0]] + tam[hijo[u][1]] + cnt[u];
    }

    int lado(int u) {
        return hijo[padre[u]][1] == u;
    }

    void rotar(int x) {
        int y = padre[x], z = padre[y];
        int d = lado(x);
        int dy = z ? lado(y) : 0;
        int b = hijo[x][d ^ 1];
        if (z) hijo[z][dy] = x;
        else raiz = x;
        padre[x] = z;
        hijo[x][d ^ 1] = y;
        padre[y] = x;
        hijo[y][d] = b;
        if (b) padre[b] = y;
        actualizar(y);
        actualizar(x);
        rotaciones++;
    }

    void splay(int x) {
        while (padre[x]) {
            int y = padre[x];
            if (padre[y]) {
                if (lado(x) == lado(y)) rotar(y);
                else rotar(x);
            }
            rotar(x);
        }
        actualizar(x);
    }

    void insertar(int x) {
        if (!raiz) {
            raiz = nuevoNodo(x, 0);
            return;
        }
        int u = raiz;
        while (true) {
            if (clave[u] == x) {
                cnt[u]++;
                splay(u);
                return;
            }
            int d = x > clave[u];
            if (!hijo[u][d]) {
                int v = nuevoNodo(x, u);
                hijo[u][d] = v;
                splay(v);
                return;
            }
            u = hijo[u][d];
        }
    }

    bool existe(int x) {
        if (!raiz) return false;
        int u = raiz, ultimo = raiz;
        while (u) {
            ultimo = u;
            if (clave[u] == x) break;
            u = hijo[u][x > clave[u]];
        }
        splay(ultimo);
        return clave[ultimo] == x;
    }

    void borrar(int x) {
        if (!existe(x)) return;
        int r = raiz;
        if (cnt[r] > 1) {
            cnt[r]--;
            actualizar(r);
            return;
        }
        int L = hijo[r][0], R = hijo[r][1];
        if (!L) {
            raiz = R;
            if (R) padre[R] = 0;
            return;
        }
        padre[L] = 0;
        raiz = L;
        int m = L;
        while (hijo[m][1]) m = hijo[m][1];
        splay(m);
        hijo[m][1] = R;
        if (R) padre[R] = m;
        actualizar(m);
    }

    int kesimo(int k) {
        if (k < 1 || k > tam[raiz]) return -1;
        int u = raiz;
        while (true) {
            int t = tam[hijo[u][0]];
            if (k <= t) {
                u = hijo[u][0];
            } else if (k <= t + cnt[u]) {
                break;
            } else {
                k -= t + cnt[u];
                u = hijo[u][1];
            }
        }
        splay(u);
        return clave[u];
    }

    int contarMenores(int x) {
        int u = raiz, ultimo = 0, res = 0;
        while (u) {
            ultimo = u;
            if (clave[u] < x) {
                res += tam[hijo[u][0]] + cnt[u];
                u = hijo[u][1];
            } else {
                u = hijo[u][0];
            }
        }
        if (ultimo) splay(ultimo);
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    SplayTree st;
    while (q--) {
        int tipo;
        cin >> tipo;
        if (tipo == 6) {
            cout << st.rotaciones << '\n';
            continue;
        }
        int x;
        cin >> x;
        if (tipo == 1) st.insertar(x);
        else if (tipo == 2) st.borrar(x);
        else if (tipo == 3) cout << (st.existe(x) ? 1 : 0) << '\n';
        else if (tipo == 4) cout << st.kesimo(x) << '\n';
        else cout << st.contarMenores(x) << '\n';
    }
    return 0;
}
