#include <bits/stdc++.h>
using namespace std;

struct VistaGeometrica {
    int n, m;
    vector<int> acceso;
    vector<vector<int>> tocados;

    VistaGeometrica(int n, vector<int> acceso) : n(n), m(acceso.size()), acceso(acceso) {}

    long long greedy() {
        vector<int> ultimo(n + 1, 0);
        tocados.assign(m + 1, {});
        long long costo = 0;
        for (int t = 1; t <= m; t++) {
            int x = acceso[t - 1];
            vector<int> fila = {x};
            int mayor = ultimo[x];
            for (int k = x - 1; k >= 1; k--) {
                if (ultimo[k] > mayor) {
                    fila.push_back(k);
                    mayor = ultimo[k];
                }
            }
            mayor = ultimo[x];
            for (int k = x + 1; k <= n; k++) {
                if (ultimo[k] > mayor) {
                    fila.push_back(k);
                    mayor = ultimo[k];
                }
            }
            for (int k : fila) ultimo[k] = t;
            sort(fila.begin(), fila.end());
            tocados[t] = fila;
            costo += fila.size();
        }
        return costo;
    }

    long long wilber1() {
        vector<int> ultimoLado(n + 2, -1);
        long long total = 0;
        for (int x : acceso) {
            int l = 1, r = n;
            while (l <= r) {
                int y = (l + r) / 2;
                if (x == y) break;
                int lado = x > y;
                if (ultimoLado[y] != -1 && ultimoLado[y] != lado) total++;
                ultimoLado[y] = lado;
                if (lado) l = y + 1;
                else r = y - 1;
            }
        }
        return total;
    }

    long long wilber2() {
        long long total = 0;
        for (int i = 0; i < m; i++) {
            int x = acceso[i];
            int izqMax = INT_MIN, derMin = INT_MAX, ultimoSigno = 0;
            for (int j = i - 1; j >= 0; j--) {
                int y = acceso[j];
                if (y == x) break;
                int signo = 0;
                if (y < x && y > izqMax) {
                    izqMax = y;
                    signo = -1;
                } else if (y > x && y < derMin) {
                    derMin = y;
                    signo = 1;
                }
                if (!signo) continue;
                if (ultimoSigno && signo != ultimoSigno) total++;
                ultimoSigno = signo;
            }
        }
        return total;
    }

    bool esArboralmenteSatisfecho() {
        vector<pair<int, int>> puntos;
        for (int t = 1; t <= m; t++)
            for (int k : tocados[t]) puntos.push_back({t, k});
        set<pair<int, int>> conjunto(puntos.begin(), puntos.end());
        for (auto &a : puntos) {
            for (auto &b : puntos) {
                if (a.first >= b.first || a.second == b.second) continue;
                int k1 = min(a.second, b.second), k2 = max(a.second, b.second);
                bool ok = false;
                for (auto &c : puntos) {
                    if (c == a || c == b) continue;
                    if (c.first >= a.first && c.first <= b.first && c.second >= k1 && c.second <= k2) {
                        ok = true;
                        break;
                    }
                }
                if (!ok) return false;
            }
        }
        return true;
    }

    void imprimir() {
        for (int t = m; t >= 1; t--) {
            string fila(n, '.');
            for (int k : tocados[t]) fila[k - 1] = 'o';
            fila[acceso[t - 1] - 1] = 'X';
            cout << setw(3) << t << ' ' << fila << '\n';
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<int> acceso(m);
    for (int &x : acceso) cin >> x;
    VistaGeometrica vg(n, acceso);
    cout << "greedy " << vg.greedy() << '\n';
    cout << "wilber1 " << vg.wilber1() << '\n';
    cout << "wilber2 " << vg.wilber2() << '\n';
    if (n <= 60 && m <= 60) {
        cout << "satisfecho " << (vg.esArboralmenteSatisfecho() ? "si" : "no") << '\n';
        vg.imprimir();
    }
    return 0;
}
