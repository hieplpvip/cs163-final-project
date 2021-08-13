#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> par;
vector<string> A;
unordered_map<string, int> mp;
unordered_map<int, vector<string>> mp_res;

int root(int v) {
  return (par[v] < 0) ? v : (par[v] = root(par[v]));
}

void join(int x, int y) {
  if ((x = root(x)) == (y = root(y))) return;
  if (par[x] > par[y]) swap(x, y);
  par[x] += par[y];
  par[y] = x;
}

void addWord(string word) {
  if (!mp.count(word)) {
    mp[word] = N++;
    par.push_back(-1);
    A.push_back(word);
  }
}

void tolower(string &s) {
  transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return std::tolower(c); });
}

int main() {
  ifstream in("synonyms.txt");
  if (!in.is_open()) {
    cout << "Please download https://raw.githubusercontent.com/hansonrobotics/hr-solr/master/synonyms.txt\n";
    return 1;
  }

  string line;
  getline(in, line);  // skip copyright line
  while (getline(in, line)) {
    string word;
    vector<string> words;
    stringstream ss(line);
    while (getline(ss, word, ',')) {
      tolower(word);
      words.push_back(word);
    }

    if (words.empty()) continue;
    addWord(words[0]);
    for (size_t i = 1; i < words.size(); ++i) {
      addWord(words[i]);
      join(mp[words[0]], mp[words[i]]);
    }
  }

  in.close();

  cout << "Found " << N << " words\n";

  for (int i = 0; i < N; ++i) {
    mp_res[root(i)].emplace_back(A[i]);
  }

  ofstream out("final_synonyms.txt");
  if (!out.is_open()) {
    cout << "Could not open file for writing!\n";
    return 1;
  }

  for (auto &p : mp_res) {
    out << p.second[0];
    for (size_t i = 1; i < p.second.size(); ++i) {
      out << ' ' << p.second[i];
    }
    out << '\n';
  }

  out.close();

  return 0;
}
