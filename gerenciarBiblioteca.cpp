#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <climits>
#include <algorithm>


using namespace std;

class Livro {
public:
    string titulo;
    string autor;
    int id;
    string categoria;  
    Livro() {} 
    Livro(const string& _titulo, const string& _autor, int _id) : titulo(_titulo), autor(_autor), id(_id) {}
};

class GrafoLivros {
public:
    
    vector<vector<bool>> disponibilidade;  
    vector<int> predecessor;  

    GrafoLivros() {
        
        disponibilidade.resize(100, vector<bool>(100, true)); 
        predecessor.resize(100, -1); 
    }
    
     void adicionarLivro(const string& titulo, const string& autor);
    void conectarLivros(int id1, int id2);
    void executarDijkstra(int idOrigem, int idDestino);
    void listarCaminho(int idOrigem, int idDestino);
    void removerLivro(int id);
    void listarLivrosConectados(int id);
    void adicionarCategoria(int id, const std::string& categoria);
    void listarLivrosPorCategoria(const std::string& categoria);
    bool verificarConexao(int id1, int id2);
    void modificarLivro(int id, const string& novoTitulo, const string& novoAutor, const string& novaCategoria);
    void registrarEmprestimo(int id);
    void listarLivrosPopulares();
    void listarLivrosEmprestados();
    
private:
    struct Aresta {
        int destino;
        int peso;
        Aresta(int _destino, int _peso) : destino(_destino), peso(_peso) {}
    };

    vector<Livro> livros;
    vector<vector<Aresta>> grafo;
    std::map<std::string, std::vector<int>> categorias;
};

void GrafoLivros::adicionarLivro(const string& titulo, const string& autor) {
    int id = livros.size();
    Livro livro(titulo, autor, id);
    livros.push_back(livro);
    grafo.emplace_back();
    cout << "Livro '" << titulo << "' adicionado com ID " << id << "." << endl;
}

void GrafoLivros::conectarLivros(int id1, int id2) {
    if (id1 >= 0 && id1 < livros.size() && id2 >= 0 && id2 < livros.size() && id1 != id2) {
        int peso = 1; 
        grafo[id1].emplace_back(id2, peso);
        grafo[id2].emplace_back(id1, peso);
    }
}

void GrafoLivros::executarDijkstra(int idOrigem, int idDestino) {
    vector<int> distancia(livros.size(), INT_MAX);
    vector<int> predecessor(livros.size(), -1);

    distancia[idOrigem] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fila_prioridade;
    fila_prioridade.push({0, idOrigem});

    while (!fila_prioridade.empty()) {
        int u = fila_prioridade.top().second;
        fila_prioridade.pop();

        for (const Aresta& aresta : grafo[u]) {
            int v = aresta.destino;
            int peso = aresta.peso;

            if (distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                predecessor[v] = u;
                fila_prioridade.push({distancia[v], v});
            }
        }
    }

    listarCaminho(idOrigem, idDestino);
}

void GrafoLivros::listarCaminho(int idOrigem, int idDestino) {
    vector<int> caminho;
    int atual = idDestino;

    while (atual != -1) {
        caminho.push_back(atual);
        atual = predecessor[atual];
    }

    if (caminho.back() == idOrigem) {
        cout << "Caminho mais curto entre '" << livros[idOrigem].titulo << "' e '" << livros[idDestino].titulo << "':" << endl;
        for (int i = caminho.size() - 1; i >= 0; --i) {
            int idLivro = caminho[i];
            cout << "ID: " << idLivro << ", Título: " << livros[idLivro].titulo << ", Autor: " << livros[idLivro].autor << endl;
        }
    } else {
        cout << "Não há caminho entre '" << livros[idOrigem].titulo << "' e '" << livros[idDestino].titulo << "'." << endl;
    }
}

void GrafoLivros::removerLivro(int id) {
    if (id >= 0 && id < livros.size()) {
        // Remova o livro do grafo e da lista de livros
        for (int i = 0; i < livros.size(); ++i) {
            grafo[i].erase(remove_if(grafo[i].begin(), grafo[i].end(), [id](const Aresta& a) { return a.destino == id; }), grafo[i].end());
        }
        livros.erase(livros.begin() + id);
        // Atualize os IDs dos livros restantes
        for (int i = 0; i < livros.size(); ++i) {
            livros[i].id = i;
        }
        cout << "Livro removido com sucesso." << endl;
    } else {
        cout << "ID de livro inválido." << endl;
    }
}

void GrafoLivros::listarLivrosConectados(int id) {
    if (id >= 0 && id < livros.size()) {
        cout << "Livros conectados a '" << livros[id].titulo << "':" << endl;
        for (const Aresta& aresta : grafo[id]) {
            const Livro& livro = livros[aresta.destino];
            cout << "ID: " << livro.id << ", Título: " << livro.titulo << ", Autor: " << livro.autor << endl;
        }
    } else {
        cout << "ID de livro inválido." << endl;
    }
}

void GrafoLivros::adicionarCategoria(int id, const std::string& categoria) {
    if (id >= 0 && id < livros.size()) {
        // Adicione a categoria ao livro
        livros[id].categoria = categoria;
        cout << "Categoria '" << categoria << "' adicionada ao livro '" << livros[id].titulo << "'." << endl;
    } else {
        cout << "ID de livro inválido." << endl;
    }
}

void GrafoLivros::listarLivrosPorCategoria(const std::string& categoria) {
    if (categorias.find(categoria) != categorias.end()) {
        cout << "Livros na categoria '" << categoria << "':" << endl;
        const std::vector<int>& livrosCategoria = categorias[categoria];
        for (int idLivro : livrosCategoria) {
            const Livro& livro = livros[idLivro];
            cout << "ID: " << livro.id << ", Título: " << livro.titulo << ", Autor: " << livro.autor << endl;
        }
    } else {
        cout << "Categoria não encontrada." << endl;
    }
}

bool GrafoLivros::verificarConexao(int id1, int id2) {
    if (id1 >= 0 && id1 < livros.size() && id2 >= 0 && id2 < livros.size()) {
        for (const Aresta& aresta : grafo[id1]) {
            if (aresta.destino == id2) {
                return true;
            }
        }
    }
    return false;
}

void GrafoLivros::modificarLivro(int id, const string& novoTitulo, const string& novoAutor, const string& novaCategoria) {
    if (id >= 0 && id < livros.size()) {
        // Modifique as informações do livro
        livros[id].titulo = novoTitulo;
        livros[id].autor = novoAutor;
        livros[id].categoria = novaCategoria;
        cout << "Informações do livro atualizadas com sucesso." << endl;
    } else {
        cout << "ID de livro inválido." << endl;
    }
}


void GrafoLivros::registrarEmprestimo(int id) {
    if (id >= 0 && id < livros.size()) {
        if (disponibilidade[id][id]) {
            disponibilidade[id][id] = false;
            cout << "Livro '" << livros[id].titulo << "' emprestado com sucesso." << endl;
        } else {
            cout << "O livro '" << livros[id].titulo << "' não está disponível para empréstimo." << endl;
        }
    } else {
        cout << "ID de livro inválido." << endl;
    }
}


void GrafoLivros::listarLivrosPopulares() {
    cout << "Livros mais populares (baseado em empréstimos):" << endl;
    vector<pair<int, int>> populares; 
    for (int i = 0; i < livros.size(); ++i) {
        int numEmprestimos = 0;
        for (int j = 0; j < livros.size(); ++j) {
            if (!disponibilidade[j][j] && verificarConexao(i, j)) {
                ++numEmprestimos;
            }
        }
        populares.emplace_back(i, numEmprestimos);
    }
    
    sort(populares.begin(), populares.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;
    });
    for (const auto& par : populares) {
        const Livro& livro = livros[par.first];
        cout << "ID: " << livro.id << ", Título: " << livro.titulo << ", Autor: " << livro.autor << ", Empréstimos: " << par.second << endl;
    }
}

void GrafoLivros::listarLivrosEmprestados() {
    cout << "Livros emprestados:" << endl;
    
    for (int i = 0; i < livros.size(); ++i) {
        if (!disponibilidade[i][i]) {
            cout << "ID: " << livros[i].id << ", Título: " << livros[i].titulo << ", Autor: " << livros[i].autor << endl;
        }
    }
}


 int main() {
     
    GrafoLivros grafoLivros;
    std::string categoria; 

    int opcao;
    do {
        cout << "\nBem Vindo a Biblioteca Universitaria:\n" << endl;
        cout << "1. Adicionar Livro" << endl;
        cout << "2. Conectar Livros" << endl;
        cout << "3. Verificar Conexão entre Livros" << endl;
        cout << "4. Modificar Livro" << endl;
        cout << "5. Registrar Empréstimo de Livro" << endl;
        cout << "6. Listar Livros Populares" << endl;
        cout << "7. Listar Livros Emprestados" << endl;
        cout << "8. Adicionar Categoria a Livro" << endl;  
        cout << "9. Listar Livros por Categoria" << endl; 
        cout << "0. Sair" << endl;
        cout << "Digite sua escolha: \n";
        cin >> opcao;
        switch (opcao) {
            case 1:
        {
            string titulo, autor;
            cout << "Digite o título do livro: ";
            cin.ignore();
            getline(cin, titulo);
            cout << "Digite o nome do autor: ";
            getline(cin, autor);
            grafoLivros.adicionarLivro(titulo, autor);
        }
        break;
    case 2:
        {
            int id1, id2;
            cout << "Digite o ID do primeiro livro: ";
            cin >> id1;
            cout << "Digite o ID do segundo livro: ";
            cin >> id2;
            grafoLivros.conectarLivros(id1, id2);
        }
        break;
            case 3:
                {
                    int id1, id2;
                    cout << "Digite o ID do primeiro livro: ";
                    cin >> id1;
                    cout << "Digite o ID do segundo livro: ";
                    cin >> id2;
                    if (grafoLivros.verificarConexao(id1, id2)) {
                        cout << "Os livros estão conectados." << endl;
                    } else {
                        cout << "Os livros não estão conectados." << endl;
                    }
                }
                break;
            case 4:
                {
                    int id;
                    string novoTitulo, novoAutor, novaCategoria;
                    cout << "Digite o ID do livro a ser modificado: ";
                    cin >> id;
                    cout << "Digite o novo título: ";
                    cin.ignore();
                    getline(cin, novoTitulo);
                    cout << "Digite o novo autor: ";
                    getline(cin, novoAutor);
                    cout << "Digite a nova categoria: ";
                    getline(cin, novaCategoria);
                    grafoLivros.modificarLivro(id, novoTitulo, novoAutor, novaCategoria);
                }
                break;
            case 5:
                {
                    int id;
                    cout << "Digite o ID do livro a ser emprestado: ";
                    cin >> id;
                    grafoLivros.registrarEmprestimo(id);
                }
                break;
            case 6:
                grafoLivros.listarLivrosPopulares();
                break;
                
             case 7:
                grafoLivros.listarLivrosEmprestados();
                break;
             case 8:
             {
                int idLivro;
                cout << "Digite o ID do livro: ";
                cin >> idLivro;
                cout << "Digite o nome da categoria: ";
                cin.ignore();
                getline(cin, categoria);
                grafoLivros.adicionarCategoria(idLivro, categoria);
                break;
             }
             
          case 9:
                {
                    std::string categoria;
                    cout << "Digite o nome da categoria: ";
                    cin.ignore();
                    getline(cin, categoria);
                    grafoLivros.listarLivrosPorCategoria(categoria);
                }
            case 0:
                cout << "Saindo do programa." << endl;
                break;
            default:
                cout << "Opção inválida. Tente novamente." << endl;
                break;
        }
    } while (opcao != 0);

    return 0;
}
