#include <bits/stdc++.h>
using namespace std;

struct ListaPersistente {
    static const int P = 2;
    enum { VALOR, ANT, SIG };

    struct Modificacion {
        int campo, valor, tiempo;
    };

    struct Nodo {
        int base[3];
        vector<Modificacion> registro;
        int reemplazo;
    };

    vector<Nodo> nodos;
    vector<int> raiz;
    int ahora = 0;
    int splits = 0;

    ListaPersistente() {
        nodos.push_back(Nodo{{0, 0, 0}, {}, 0});
        raiz.push_back(0);
    }

    int nuevoNodo(int valor, int ant, int sig) {
        nodos.push_back(Nodo{{valor, ant, sig}, {}, 0});
        return (int)nodos.size() - 1;
    }

    int actual(int u) {
        while (u && nodos[u].reemplazo) u = nodos[u].reemplazo;
        return u;
    }

    int leer(int u, int campo, int t) {
        auto &reg = nodos[u].registro;
        for (int i = (int)reg.size() - 1; i >= 0; i--)
            if (reg[i].campo == campo && reg[i].tiempo <= t) return reg[i].valor;
        return nodos[u].base[campo];
    }

    void escribir(int u, int campo, int valor) {
        u = actual(u);
        if (campo != VALOR) valor = actual(valor);
        auto &reg = nodos[u].registro;
        for (auto &m : reg) {
            if (m.campo == campo && m.tiempo == ahora) {
                m.valor = valor;
                return;
            }
        }
        if ((int)reg.size() < 2 * P) {
            reg.push_back({campo, valor, ahora});
            return;
        }
        splits++;
        int v = leer(u, VALOR, ahora), a = leer(u, ANT, ahora), s = leer(u, SIG, ahora);
        if (campo == VALOR) v = valor;
        if (campo == ANT) a = valor;
        if (campo == SIG) s = valor;
        int w = nuevoNodo(v, a, s);
        nodos[u].reemplazo = w;
        if (a) escribir(a, SIG, w);
        else raiz[ahora] = w;
        if (s) escribir(s, ANT, w);
    }

    int buscarPosicion(int k, int t) {
        int u = raiz[t];
        for (int i = 1; i < k && u; i++) u = leer(u, SIG, t);
        return u;
    }

    void nuevaVersion() {
        ahora++;
        raiz.push_back(raiz[ahora - 1]);
    }

    void insertar(int k, int x) {
        nuevaVersion();
        int a = k ? buscarPosicion(k, ahora) : 0;
        if (k && !a) return;
        int s = a ? leer(a, SIG, ahora) : raiz[ahora];
        int nuevo = nuevoNodo(x, a, s);
        if (a) escribir(a, SIG, nuevo);
        else raiz[ahora] = nuevo;
        if (s) escribir(s, ANT, nuevo);
    }

    void borrar(int k) {
        nuevaVersion();
        int u = k >= 1 ? buscarPosicion(k, ahora) : 0;
        if (!u) return;
        int a = leer(u, ANT, ahora), s = leer(u, SIG, ahora);
        if (a) escribir(a, SIG, s);
        else raiz[ahora] = actual(s);
        if (s) escribir(s, ANT, a);
    }

    void modificar(int k, int x) {
        nuevaVersion();
        int u = k >= 1 ? buscarPosicion(k, ahora) : 0;
        if (!u) return;
        escribir(u, VALOR, x);
    }

    vector<int> obtenerLista(int t) {
        vector<int> res;
        for (int u = raiz[t]; u; u = leer(u, SIG, t)) res.push_back(leer(u, VALOR, t));
        return res;
    }

    int kesimo(int k, int t) {
        int u = k >= 1 ? buscarPosicion(k, t) : 0;
        return u ? leer(u, VALOR, t) : -1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    ListaPersistente lista;
    while (q--) {
        int tipo;
        cin >> tipo;
        if (tipo == 1) {
            int k, x;
            cin >> k >> x;
            lista.insertar(k, x);
        } else if (tipo == 2) {
            int k;
            cin >> k;
            lista.borrar(k);
        } else if (tipo == 3) {
            int k, x;
            cin >> k >> x;
            lista.modificar(k, x);
        } else if (tipo == 4) {
            int v, k;
            cin >> v >> k;
            cout << lista.kesimo(k, v) << '\n';
        } else if (tipo == 5) {
            int v;
            cin >> v;
            vector<int> res = lista.obtenerLista(v);
            cout << res.size();
            for (int x : res) cout << ' ' << x;
            cout << '\n';
        } else {
            cout << lista.nodos.size() - 1 << ' ' << lista.splits << '\n';
        }
    }
    return 0;
}
