#include <bits/stdc++.h>
using namespace std;

struct FibonacciHeap {
    vector<long long> clave;
    vector<int> padre, hijo, izq, der, grado;
    vector<bool> marca, vivo;
    vector<int> minimo, tam;

    vector<int> grupoDe, padreGrupo, duenoGrupo, grupoActual;

    int nuevoGrupo(int h) {
        padreGrupo.push_back(padreGrupo.size());
        duenoGrupo.push_back(h);
        return (int)padreGrupo.size() - 1;
    }

    int buscarGrupo(int g) {
        while (padreGrupo[g] != g) {
            padreGrupo[g] = padreGrupo[padreGrupo[g]];
            g = padreGrupo[g];
        }
        return g;
    }

    int heapDe(int u) {
        return duenoGrupo[buscarGrupo(grupoDe[u])];
    }

    FibonacciHeap(int cantidadHeaps) {
        minimo.assign(cantidadHeaps + 1, 0);
        tam.assign(cantidadHeaps + 1, 0);
        grupoActual.assign(cantidadHeaps + 1, 0);
        for (int h = 0; h <= cantidadHeaps; h++) grupoActual[h] = nuevoGrupo(h);
        grupoDe.push_back(0);
        clave.push_back(0);
        padre.push_back(0);
        hijo.push_back(0);
        izq.push_back(0);
        der.push_back(0);
        grado.push_back(0);
        marca.push_back(false);
        vivo.push_back(false);
    }

    void agregarRaiz(int h, int x) {
        padre[x] = 0;
        int m = minimo[h];
        if (!m) {
            izq[x] = der[x] = x;
            minimo[h] = x;
            return;
        }
        int r = der[m];
        der[m] = x;
        izq[x] = m;
        der[x] = r;
        izq[r] = x;
        if (clave[x] < clave[m]) minimo[h] = x;
    }

    void quitarDeLista(int x) {
        der[izq[x]] = der[x];
        izq[der[x]] = izq[x];
    }

    int insertar(int h, long long x) {
        clave.push_back(x);
        padre.push_back(0);
        hijo.push_back(0);
        izq.push_back(0);
        der.push_back(0);
        grado.push_back(0);
        marca.push_back(false);
        vivo.push_back(true);
        grupoDe.push_back(grupoActual[h]);
        int u = (int)clave.size() - 1;
        agregarRaiz(h, u);
        tam[h]++;
        return u;
    }

    void unir(int a, int b) {
        if (a == b) return;
        padreGrupo[buscarGrupo(grupoActual[b])] = buscarGrupo(grupoActual[a]);
        grupoActual[b] = nuevoGrupo(b);
        if (!minimo[b]) return;
        if (!minimo[a]) {
            minimo[a] = minimo[b];
        } else {
            int ma = minimo[a], mb = minimo[b];
            int ra = der[ma], lb = izq[mb];
            der[ma] = mb;
            izq[mb] = ma;
            der[lb] = ra;
            izq[ra] = lb;
            if (clave[mb] < clave[ma]) minimo[a] = mb;
        }
        tam[a] += tam[b];
        minimo[b] = 0;
        tam[b] = 0;
    }

    void enlazar(int y, int x) {
        padre[y] = x;
        marca[y] = false;
        int c = hijo[x];
        if (!c) {
            hijo[x] = y;
            izq[y] = der[y] = y;
        } else {
            int r = der[c];
            der[c] = y;
            izq[y] = c;
            der[y] = r;
            izq[r] = y;
        }
        grado[x]++;
    }

    void consolidar(int h) {
        int m = minimo[h];
        if (!m) return;
        vector<int> raices;
        int u = m;
        do {
            raices.push_back(u);
            u = der[u];
        } while (u != m);
        vector<int> porGrado(64, 0);
        for (int w : raices) {
            int x = w, d = grado[x];
            while (porGrado[d]) {
                int y = porGrado[d];
                if (clave[y] < clave[x]) swap(x, y);
                enlazar(y, x);
                porGrado[d] = 0;
                d++;
            }
            porGrado[d] = x;
        }
        minimo[h] = 0;
        for (int x : porGrado)
            if (x) agregarRaiz(h, x);
    }

    void extraerMin(int h) {
        int z = minimo[h];
        if (!z) return;
        int c = hijo[z];
        if (c) {
            vector<int> hijos;
            int u = c;
            do {
                hijos.push_back(u);
                u = der[u];
            } while (u != c);
            for (int x : hijos) agregarRaiz(h, x);
            hijo[z] = 0;
        }
        if (der[z] == z) {
            minimo[h] = 0;
        } else {
            minimo[h] = der[z];
            quitarDeLista(z);
        }
        vivo[z] = false;
        tam[h]--;
        consolidar(h);
    }

    void cortar(int h, int x, int y) {
        if (der[x] == x) {
            hijo[y] = 0;
        } else {
            if (hijo[y] == x) hijo[y] = der[x];
            quitarDeLista(x);
        }
        grado[y]--;
        agregarRaiz(h, x);
        marca[x] = false;
    }

    void disminuirClave(int id, long long k) {
        if (id <= 0 || id >= (int)vivo.size() || !vivo[id] || k > clave[id]) return;
        int h = heapDe(id);
        clave[id] = k;
        int y = padre[id];
        if (y && clave[id] < clave[y]) {
            cortar(h, id, y);
            while (true) {
                int z = padre[y];
                if (!z) break;
                if (!marca[y]) {
                    marca[y] = true;
                    break;
                }
                cortar(h, y, z);
                y = z;
            }
        }
        if (clave[id] < clave[minimo[h]]) minimo[h] = id;
    }

    void borrar(int id) {
        if (id <= 0 || id >= (int)vivo.size() || !vivo[id]) return;
        int h = heapDe(id);
        disminuirClave(id, LLONG_MIN);
        extraerMin(h);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, q;
    cin >> m >> q;
    FibonacciHeap heap(m);
    while (q--) {
        int tipo;
        cin >> tipo;
        if (tipo == 1) {
            int h;
            long long x;
            cin >> h >> x;
            heap.insertar(h, x);
        } else if (tipo == 2) {
            int h;
            cin >> h;
            if (!heap.minimo[h]) cout << "vacio\n";
            else cout << heap.clave[heap.minimo[h]] << '\n';
        } else if (tipo == 3) {
            int h;
            cin >> h;
            heap.extraerMin(h);
        } else if (tipo == 4) {
            int a, b;
            cin >> a >> b;
            heap.unir(a, b);
        } else if (tipo == 5) {
            int id;
            long long k;
            cin >> id >> k;
            heap.disminuirClave(id, k);
        } else {
            int id;
            cin >> id;
            heap.borrar(id);
        }
    }
    return 0;
}
