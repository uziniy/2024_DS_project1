#include <iostream>
#include <fstream>
#include <string>

class SubtitleBSTNode {
public:
    std::string time;  // 자막 시간
    std::string content;  // 자막 내용
    SubtitleBSTNode* left;  // 왼쪽 자식 노드
    SubtitleBSTNode* right;  // 오른쪽 자식 노드

    SubtitleBSTNode(const std::string& t, const std::string& c)
        : time(t), content(c), left(nullptr), right(nullptr) {}
};

// 자막 정보를 저장하는 BST 클래스
class SubtitleBST {
private:
    SubtitleBSTNode* root;

    // 노드를 삽입하는 재귀 함수
    SubtitleBSTNode* insert(SubtitleBSTNode* node, const std::string& time, const std::string& content) {
        if (!node) return new SubtitleBSTNode(time, content);
        if (time < node->time) node->left = insert(node->left, time, content);
        else node->right = insert(node->right, time, content);
        return node;
    }

    // 특정 시간 구간의 노드들을 찾아 섹션에 추가하는 함수
    void findInRange(SubtitleBSTNode* node, const std::string& start, const std::string& end, SubtitleBSTNode*& sectionHead) const {
        if (!node) return;
        if (node->time >= start && node->time <= end) {
            SubtitleBSTNode* newNode = new SubtitleBSTNode(node->time, node->content);
            newNode->right = sectionHead;
            sectionHead = newNode;
        }
        if (node->time > start) findInRange(node->left, start, end, sectionHead);
        if (node->time < end) findInRange(node->right, start, end, sectionHead);
    }

    void inorderPrint(SubtitleBSTNode* node) const {
        if (!node) return;
        inorderPrint(node->left);
        std::cout << node->time << " - " << node->content << std::endl;
        inorderPrint(node->right);
    }

    SubtitleBSTNode* deleteNode(SubtitleBSTNode* node, const std::string& time) {
        if (!node) return nullptr;
        if (time < node->time) {
            node->left = deleteNode(node->left, time);
        } else if (time > node->time) {
            node->right = deleteNode(node->right, time);
        } else {
            if (!node->left) {
                SubtitleBSTNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                SubtitleBSTNode* temp = node->left;
                delete node;
                return temp;
            } else {
                SubtitleBSTNode* temp = findMin(node->right);
                node->time = temp->time;
                node->content = temp->content;
                node->right = deleteNode(node->right, temp->time);
            }
        }
        return node;
    }

    SubtitleBSTNode* findMin(SubtitleBSTNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    SubtitleBSTNode* deleteUnder(SubtitleBSTNode* node, const std::string& time) {
        if (!node) return nullptr;
        if (node->time < time) {
            SubtitleBSTNode* rightSubtree = deleteUnder(node->right, time);
            delete node;
            return rightSubtree;
        } else {
            node->left = deleteUnder(node->left, time);
            return node;
        }
    }

public:
    SubtitleBST() : root(nullptr) {}

    void insert(const std::string& time, const std::string& content) {
        root = insert(root, time, content);
    }

    void print() const {
        if (!root) {
            std::cout << "===== ERROR =====\n300\n===============\n";
            return;
        }
        std::cout << "===== PRINT =====\n";
        inorderPrint(root);
        std::cout << "===============\n";
    }

    void deleteEqual(const std::string& time) {
        root = deleteNode(root, time);
        if (!root) {
            std::cout << "===== ERROR =====\n500\n===============\n";
        } else {
            std::cout << "===== DELETE EQUAL =====\nSuccess\n===============\n";
        }
    }

    void deleteUnder(const std::string& time) {
        root = deleteUnder(root, time);
        std::cout << "===== DELETE UNDER =====\nSuccess\n===============\n";
    }

    SubtitleBSTNode* getSection(const std::string& start, const std::string& end) const {
        SubtitleBSTNode* sectionHead = nullptr;
        findInRange(root, start, end, sectionHead);
        return sectionHead;
    }
};

// 섹션 노드를 저장하는 리스트 클래스
class SectionList {
private:
    struct SectionListNode {
        int sectionNum;
        SubtitleBSTNode* subtitles;  // 자막 리스트의 첫 번째 노드
        SectionListNode* next;

        SectionListNode(int num, SubtitleBSTNode* subs) : sectionNum(num), subtitles(subs), next(nullptr) {}
    };

    SectionListNode* head;

public:
    SectionList() : head(nullptr) {}

    void addSection(int sectionNum, SubtitleBSTNode* subtitles) {
        SectionListNode* newSection = new SectionListNode(sectionNum, subtitles);
        newSection->next = head;
        head = newSection;
        std::cout << "===== SECTION =====\nSuccess\n===============\n";
    }

    void printSection(int sectionNum) const {
        SectionListNode* current = head;
        while (current) {
            if (current->sectionNum == sectionNum) {
                std::cout << "===== PRINT SECTION " << sectionNum << " =====\n";
                SubtitleBSTNode* subtitle = current->subtitles;
                while (subtitle) {
                    std::cout << subtitle->time << " - " << subtitle->content << std::endl;
                    subtitle = subtitle->right;
                }
                std::cout << "===============\n";
                return;
            }
            current = current->next;
        }
        std::cout << "===== ERROR =====\n400\n===============\n";
    }
};

class Manager {
private:
    SubtitleBST bst;
    SectionList sectionList;

public:
    void loadSubtitles(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "===== ERROR =====\n100\n===============\n";
            return;
        }
        std::string line;
        while (getline(file, line)) {
            int pos = line.find(' ');
            if (pos == -1) continue;

            std::string time = line.substr(0, pos);
            std::string content = line.substr(pos + 1);
            bst.insert(time, content);
        }
        file.close();
        std::cout << "===== LOAD =====\nLoad Complete\n===============\n";
    }

    void printBST() {
        bst.print();
    }

    void section(int sectionNum, const std::string& startTime, const std::string& endTime) {
        SubtitleBSTNode* sectionHead = bst.getSection(startTime, endTime);
        if (!sectionHead) {
            std::cout << "===== ERROR =====\n400\n===============\n";
        } else {
            sectionList.addSection(sectionNum, sectionHead);
        }
    }

    void printSection(int sectionNum) {
        sectionList.printSection(sectionNum);
    }

    void executeCommands(const std::string& commandFile) {
        std::ifstream file(commandFile);
        if (!file.is_open()) {
            std::cout << "===== ERROR =====\n1000\n===============\n";
            return;
        }
        std::string command;
        while (file >> command) {
            if (command == "LOAD") {
                loadSubtitles("subtitle.txt");
            } else if (command == "PRINT") {
                if (file.peek() == '\n' || file.eof()) {
                    printBST();
                } else {
                    int sectionNum;
                    file >> sectionNum;
                    printSection(sectionNum);
                }
            } else if (command == "SECTION") {
                int sectionNum;
                std::string startTime, endTime;
                file >> sectionNum >> startTime >> endTime;
                section(sectionNum, startTime, endTime);
            } else if (command == "DELETE") {
                std::string type, time;
                file >> type >> time;
                if (type == "EQUAL") {
                    bst.deleteEqual(time);
                } else if (type == "UNDER") {
                    bst.deleteUnder(time);
                } else {
                    std::cout << "===== ERROR =====\n1000\n===============\n";
                }
            } else {
                std::cout << "===== ERROR =====\n1000\n===============\n";
            }
        }
        file.close();
    }
};

int main(int argc, const char* argv[]) {
    Manager manager;
    manager.executeCommands("command.txt");
    return 0;
}
