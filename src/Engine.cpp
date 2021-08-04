#include "Engine.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#include "Global.h"
#include "QueryParser.h"
#include "Utils.h"
#include <fstream>
#include <sstream>

void Engine::processQuery(const string& query, vector<QueryResult>& final_res) {
	auto mergeOccurrences = [](vector<QueryResult>& A, vector<QueryResult>& B) {
		vector<QueryResult> C;
		for (int i = 0, j = 0; i < (int)A.size() || j < (int)B.size();) {
			if (j == (int)B.size() || (i < (int)A.size() && A[i].fileID < B[j].fileID)) {
				C.push_back(A[i]);
			}
			else if (i == (int)A.size() || (j < (int)B.size() && A[i].fileID > B[j].fileID)) {
				C.push_back(B[j]);
				++j;
			}
			else {
				// A[i].fileID == B[j].fileID
				C.push_back({ A[i].fileID, A[i].score + B[j].score, {} });
				auto& pos = C.back().pos;
				pos.resize(A[i].pos.size() + B[j].pos.size());
				std::merge(A[i].pos.begin(), A[i].pos.end(), B[j].pos.begin(), B[j].pos.end(), pos.begin());
				pos.resize(std::unique(pos.begin(), pos.end()) - pos.begin());
				++i, ++j;
			}
		}
		A.swap(C);
	};

	auto intersectOccurrences = [](vector<QueryResult>& A, vector<pair<int, vector<int>>>& B) {
		vector<QueryResult> C;
		for (int i = 0, j = 0; i < (int)A.size() && j < (int)B.size();) {
			if (A[i].fileID < B[j].first) {
				++i;
			}
			else if (A[i].fileID > B[j].first) {
				++j;
			}
			else {
				// A[i].fileID == B[j].first
				C.push_back({ A[i].fileID, A[i].score + (int)B[j].second.size(), {} });
				auto& pos = C.back().pos;
				pos.resize(A[i].pos.size() + B[j].second.size());
				std::merge(A[i].pos.begin(), A[i].pos.end(), B[j].second.begin(), B[j].second.end(), pos.begin());
				pos.resize(std::unique(pos.begin(), pos.end()) - pos.begin());
				++i, ++j;
			}
		}
		A.swap(C);
	};

	auto filterOccurrencesByFileID = [](vector<QueryResult>& A, vector<int> fileIDs, bool remove) {
		std::unordered_set<int> fileIDSet(fileIDs.begin(), fileIDs.end());
		vector<QueryResult> C;
		for (auto& res : A) {
			if (fileIDSet.count(res.fileID) != remove) {
				C.push_back(res);
			}
		}
		A.swap(C);
	};

	vector<vector<QueryParser::QueryClause>> groups;
	QueryParser::parseQueryString(query, groups);
	// process each group
	for (auto& group : groups) {
		vector<QueryResult> res;
		for (int i = 0; i < Global::numFiles; ++i) {
			res.push_back({ i, 0, {} });
		}
		for (auto& clause : group) {
			switch (clause.type) {
			case QueryParser::QueryType::INCLUDE: {
				auto tmp = processInclude(clause.keyword);
				intersectOccurrences(res, tmp);
				break;
			}

			case QueryParser::QueryType::EXCLUDE: {
				auto tmp = processExclude(clause.keyword);
				filterOccurrencesByFileID(res, tmp, true);
				break;
			}

			case QueryParser::QueryType::IN_TITLE: {
				auto tmp = processInTitle(clause.keyword);
				//intersectOccurrences(res, tmp);  // TODO
				break;
			}

			case QueryParser::QueryType::FILETYPE: {
				auto tmp = processFileType(clause.keyword);
				// filterOccurrencesByFileID(res, tmp, false);  // TODO
				break;
			}

			case QueryParser::QueryType::EXACT_MATCH: {
				auto tmp = processExactMatch(clause.keyword);
				//intersectOccurrences(res, tmp);  // TODO
				break;
			}

			case QueryParser::QueryType::NUMBER_RANGE: {
				auto tmp = processNumberRange(clause.keyword);
				//intersectOccurrences(res, tmp);  // TODO
				break;
			}

			case QueryParser::QueryType::SYNONYM: {
				auto tmp= processSynonym(clause.keyword);
				for(auto& temp: tmp)
					intersectOccurrences(res, temp);
				break;	
			}
			}
		}

		mergeOccurrences(final_res, res);
	}

	// sort result by score
	std::sort(final_res.begin(), final_res.end(), [](const QueryResult& a, const QueryResult& b) {
		return a.score > b.score;
		});
}

void Engine::displayQueryResult(const string& query, const vector<QueryResult>& final_res) {
	std::cout << "Searched for: " << query << '\n';
	std::cout << "Found " << final_res.size() << " results\n";
	int k = std::min(10, (int)final_res.size());
	if (k == 0) {
		waitForEnter();
		return;
	}

	std::cout << "Top " << k << " result(s):\n\n";
	for (int i = 0; i < k; ++i) {
		displayFileResult(final_res[i]);
	}
	waitForEnter();
}

void Engine::displayFileResult(const QueryResult& res) {
	setTextColor(TextColor::GREEN);
	std::cout << "File " << Global::filesList[res.fileID] << " with score " << res.score << '\n';
	setTextColor(TextColor::WHITE);

	int L = -1, R = -1, sz = Global::fileContentWords[res.fileID].size();
	if (sz <= 100) {
		L = 0;
		R = sz - 1;
	}
	else {
		int max_words = 0;
		for (int x : res.pos) {
			int l = std::max(0, x - 9);
			int r = std::min(l + 99, sz - 1);
			int k = std::upper_bound(res.pos.begin(), res.pos.end(), r) -
				std::lower_bound(res.pos.begin(), res.pos.end(), l);
			if (k > max_words) {
				max_words = k,
					L = l;
				R = r;
			}
		}
	}

	for (int i = L; i <= R; ++i) {
		auto it = std::lower_bound(res.pos.begin(), res.pos.end(), i);
		if (it != res.pos.end() && *it == i) {
			setTextColor(TextColor::RED);
			std::cout << Global::fileContentWords[res.fileID][i];
			setTextColor(TextColor::WHITE);
		}
		else {
			std::cout << Global::fileContentWords[res.fileID][i];
		}
		std::cout << " \n"[i == R];
	}
	std::cout << '\n';
}

vector<pair<int, vector<int>>> Engine::processInclude(const string& keyword) {
	cdebug << "[Engine::processInclude] " << keyword << '\n';
	TrieNode* node = Global::trieContent.findWord(keyword);
	if (node == nullptr) {
		return {};
	}

	vector<pair<int, vector<int>>> res;
	vector<pair<int, int>> occurrences = node->occurrences;
	sort(occurrences.begin(), occurrences.end());

	int lastFileID = occurrences[0].first;
	res.push_back({ lastFileID, {} });
	for (auto [fileID, pos] : occurrences) {
		if (fileID != lastFileID) {
			lastFileID = fileID;
			res.push_back({ lastFileID, {} });
		}
		res.back().second.emplace_back(pos);
	}

	if (Global::verbose) {
		for (auto& [fileID, posArr] : res) {
			std::cout << "File " << fileID << ":";
			for (int pos : posArr) std::cout << ' ' << pos;
			std::cout << '\n';
		}
	}

	return res;
}

vector<int> Engine::processExclude(const string& keyword) {
	cdebug << "[Engine::processExclude] " << keyword << '\n';
	TrieNode* node = Global::trieContent.findWord(keyword);
	if (node == nullptr)
		return {};
	vector<int> res;
	res.push_back(node->occurrences[0].first);
	for (auto i : node->occurrences)
		if(i.first!=res.back())
		res.push_back(i.first);
	return res;
}

vector<pair<int, vector<int>>> Engine::processInTitle(const string& keyword) {
	cdebug << "[Engine::processInTitle] " << keyword << '\n';
	return {};
}

vector<int> Engine::processFileType(const string& keyword) {
	cdebug << "[Engine::processFileType] " << keyword << '\n';
	return {};
}

vector<pair<int, vector<int>>> Engine::processExactMatch(const string& keyword) {
	// also WILDCARD
	cdebug << "[Engine::processExactMatch] " << keyword << '\n';
	return {};
}

vector<pair<int, vector<int>>> Engine::processNumberRange(const string& keyword) {
	cdebug << "[Engine::processNumberRange] " << keyword << '\n';
	return {};
	// Extract number range: x is first number, y is second number
	int i = 1;
	int posX = 1;
	std::string x, y;
	while (posX < keyword.length() && keyword[posX] != '.')
		posX++;
	x = keyword.substr(i, posX - 1);
	int posY = posX + 2;
	y = keyword.substr(posY, keyword.length() - 1 - posY);

	// Process number range
}

vector<vector<pair<int, vector<int>>>> Engine:: processSynonym(const string& keyword){
	cdebug << "[Engine::processSynonym] " << keyword << '\n';
	TrieNode* node = Global::trieSynWord.findWord(keyword);
	if(node==nullptr)
		return {};
	std::ifstream fin("data/testsynonym.txt");
	if (!fin.is_open())
		return {};
	vector<string> res;
	res.push_back(keyword);
	vector<pair<int, int>> occurrences = node->occurrences;
	string str;
	for (auto [fileID, pos] : occurrences) {
		fin.seekg(pos-keyword.size()-5,fin.beg);
		fin >> str;
		if (str == "KEY:") {
			getline(fin, str);
			getline(fin, str);
			std::stringstream ss(str);
			ss >> str;
			while (ss >> str)
				res.push_back(str);
			break;
		}
	}
	fin.close();
	vector<vector<pair<int, vector<int>>>> result;
	for (int i = 0; i < res.size(); i++)
	{
		vector<pair<int, vector<int>>> temp;
		temp = Engine::processInclude(res[i]);
		result.push_back(temp);
	}
	return result;
}
