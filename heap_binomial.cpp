#include <bits/stdc++.h>
using namespace std;

struct HeapBinomial {
    vector<long long> clave;
    vector<int> padre, hijo, hermano, grado;
    vector<int> nodoDe, handleDe;
    vector<bool> vivo;
    vector<int> raiz;

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

    HeapBinomial(int cantidadHeaps) {
        raiz.assign(cantidadHeaps + 1, 0);
        grupoActual.assign(cantidadHeaps + 1, 0);
        for (int h = 0; h <= cantidadHeaps; h++) grupoActual[h] = nuevoGrupo(h);
        grupoDe.push_back(0);
        clave.push_back(0);
        padre.push_back(0);
        hijo.push_back(0);
        hermano.push_back(0);
        grado.push_back(0);
        nodoDe.push_back(0);
        handleDe.push_back(0);
        vivo.push_back(false);
    }

    void enlazar(int y, int z) {
        padre[y] = z;
        hermano[y] = hijo[z];
        hijo[z] = y;
        grado[z]++;
    }

    int mezclarListas(int a, int b) {
        int cabeza = 0, cola = 0;
        while (a || b) {
            int u;
            if (!b || (a && grado[a] <= grado[b])) {
                u = a;
                a = hermano[a];
            } else {
                u = b;
                b = hermano[b];
            }
            if (!cabeza) cabeza = u;
            else hermano[cola] = u;
            cola = u;
        }
        if (cola) hermano[cola] = 0;
        return cabeza;
    }

    int unirListas(int a, int b) {
        int cabeza = mezclarListas(a, b);
        if (!cabeza) return 0;
        int prev = 0, x = cabeza, sig = hermano[x];
        while (sig) {
            if (grado[x] != grado[sig] || (hermano[sig] && grado[hermano[sig]] == grado[x])) {
                prev = x;
                x = sig;
            } else if (clave[x] <= clave[sig]) {
                hermano[x] = hermano[sig];
                enlazar(sig, x);
            } else {
                if (!prev) cabeza = sig;
                else hermano[prev] = sig;
                enlazar(x, sig);
                x = sig;
            }
            sig = hermano[x];
        }
        return cabeza;
    }

    int insertar(int h, long long x) {
        clave.push_back(x);
        padre.push_back(0);
        hijo.push_back(0);
        hermano.push_back(0);
        grado.push_back(0);
        int u = (int)clave.size() - 1;
        nodoDe.push_back(u);
        handleDe.push_back(u);
        vivo.push_back(true);
        grupoDe.push_back(grupoActual[h]);
        raiz[h] = unirListas(raiz[h], u);
        return u;
    }

    int nodoMinimo(int h) {
        int mejor = 0;
        for (int u = raiz[h]; u; u = hermano[u])
            if (!mejor || clave[u] < clave[mejor]) mejor = u;
        return mejor;
    }

    bool vacio(int h) {
        return raiz[h] == 0;
    }

    long long minimo(int h) {
        return clave[nodoMinimo(h)];
    }

    void extraerMin(int h) {
        int x = nodoMinimo(h);
        if (!x) return;
        if (raiz[h] == x) {
            raiz[h] = hermano[x];
        } else {
            int u = raiz[h];
            while (hermano[u] != x) u = hermano[u];
            hermano[u] = hermano[x];
        }
        int c = hijo[x], invertida = 0;
        while (c) {
            int siguiente = hermano[c];
            hermano[c] = invertida;
            padre[c] = 0;
            invertida = c;
            c = siguiente;
        }
        vivo[handleDe[x]] = false;
        raiz[h] = unirListas(raiz[h], invertida);
    }

    void unir(int a, int b) {
        if (a == b) return;
        raiz[a] = unirListas(raiz[a], raiz[b]);
        raiz[b] = 0;
        padreGrupo[buscarGrupo(grupoActual[b])] = buscarGrupo(grupoActual[a]);
        grupoActual[b] = nuevoGrupo(b);
    }

    void disminuirClave(int id, long long k) {
        if (id <= 0 || id >= (int)vivo.size() || !vivo[id]) return;
        int y = nodoDe[id];
        if (k > clave[y]) return;
        clave[y] = k;
        int z = padre[y];
        while (z && clave[y] < clave[z]) {
            swap(clave[y], clave[z]);
            int hy = handleDe[y], hz = handleDe[z];
            swap(handleDe[y], handleDe[z]);
            nodoDe[hy] = z;
            nodoDe[hz] = y;
            y = z;
            z = padre[y];
        }
    }

    void borrar(int id) {
        if (id <= 0 || id >= (int)vivo.size() || !vivo[id]) return;
        int h = heapDe(nodoDe[id]);
        disminuirClave(id, LLONG_MIN);
        extraerMin(h);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, q;
    cin >> m >> q;
    HeapBinomial heap(m);
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
            if (heap.vacio(h)) cout << "vacio\n";
            else cout << heap.minimo(h) << '\n';
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
