#include "StorageJson.hpp"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace {

// --- minimal JSON helpers (only what we write/read) ---
std::string jsonEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '"':  out += "\\\""; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

bool jsonUnescapeInto(const std::string& in, std::string& out) {
    out.clear();
    out.reserve(in.size());
    for (size_t i = 0; i < in.size(); i++) {
        char c = in[i];
        if (c != '\\') { out += c; continue; }
        if (i + 1 >= in.size()) return false;
        char n = in[++i];
        switch (n) {
            case '\\': out += '\\'; break;
            case '"':  out += '"';  break;
            case 'n':  out += '\n'; break;
            case 'r':  out += '\r'; break;
            case 't':  out += '\t'; break;
            default: return false;
        }
    }
    return true;
}

enum class TokType { LBRACKET, RBRACKET, LBRACE, RBRACE, COMMA, COLON, STRING, NUMBER, END, INVALID };

struct Token {
    TokType type{TokType::INVALID};
    std::string text; 
};

struct Tokenizer {
    const std::string& s;
    size_t i{0};

    explicit Tokenizer(const std::string& src) : s(src) {}

    void skipWs() {
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) i++;
    }

    Token next() {
        skipWs();
        if (i >= s.size()) return {TokType::END, ""};

        char c = s[i++];
        switch (c) {
            case '[': return {TokType::LBRACKET, ""};
            case ']': return {TokType::RBRACKET, ""};
            case '{': return {TokType::LBRACE, ""};
            case '}': return {TokType::RBRACE, ""};
            case ',': return {TokType::COMMA, ""};
            case ':': return {TokType::COLON, ""};
            case '"': {
                // parse JSON string, store raw with escapes preserved (unescape later)
                std::string raw;
                while (i < s.size()) {
                    char ch = s[i++];
                    if (ch == '"') break;
                    if (ch == '\\') {
                        if (i >= s.size()) return {TokType::INVALID, ""};
                        char esc = s[i++];
                        raw += '\\';
                        raw += esc;
                    } else {
                        raw += ch;
                    }
                }
                return {TokType::STRING, raw};
            }
            default: {
                if (std::isdigit(static_cast<unsigned char>(c)) || c == '-') {
                    std::string num(1, c);
                    while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) {
                        num += s[i++];
                    }
                    return {TokType::NUMBER, num};
                }
                return {TokType::INVALID, std::string(1, c)};
            }
        }
    }
};

struct Parser {
    Tokenizer tz;
    Token cur;

    explicit Parser(const std::string& src) : tz(src) { cur = tz.next(); }

    bool eat(TokType t) {
        if (cur.type != t) return false;
        cur = tz.next();
        return true;
    }

    bool expect(TokType t) { return eat(t); }

    bool parseString(std::string& out) {
        if (cur.type != TokType::STRING) return false;
        std::string unesc;
        if (!jsonUnescapeInto(cur.text, unesc)) return false;
        out = unesc;
        cur = tz.next();
        return true;
    }

    bool parseInt(int& out) {
        if (cur.type != TokType::NUMBER) return false;
        try { out = std::stoi(cur.text); }
        catch (...) { return false; }
        cur = tz.next();
        return true;
    }

    bool parseBook(Book& b) {
        if (!expect(TokType::LBRACE)) return false;

        std::unordered_map<std::string, std::string> strFields;
        std::unordered_map<std::string, int> intFields;

        if (cur.type == TokType::RBRACE) return false;

        while (true) {
            std::string key;
            if (!parseString(key)) return false;
            if (!expect(TokType::COLON)) return false;

            if (key == "title" || key == "author") {
                std::string val;
                if (!parseString(val)) return false;
                strFields[key] = val;
            } else if (key == "id" || key == "year") {
                int val;
                if (!parseInt(val)) return false;
                intFields[key] = val;
            } else {
                // unknown field: consume a string or number
                if (cur.type == TokType::STRING) {
                    std::string tmp;
                    if (!parseString(tmp)) return false;
                } else if (cur.type == TokType::NUMBER) {
                    int tmp;
                    if (!parseInt(tmp)) return false;
                } else {
                    return false;
                }
            }

            if (eat(TokType::COMMA)) continue;
            if (eat(TokType::RBRACE)) break;
            return false;
        }

        if (!intFields.count("id") || !intFields.count("year") ||
            !strFields.count("title") || !strFields.count("author")) return false;

        b.id = intFields["id"];
        b.year = intFields["year"];
        b.title = strFields["title"];
        b.author = strFields["author"];
        return true;
    }

    bool parseArray(std::vector<Book>& books) {
        books.clear();
        if (!expect(TokType::LBRACKET)) return false;

        if (eat(TokType::RBRACKET)) return true;

        while (true) {
            Book b;
            if (!parseBook(b)) return false;
            books.push_back(std::move(b));

            if (eat(TokType::COMMA)) continue;
            if (eat(TokType::RBRACKET)) break;
            return false;
        }
        return true;
    }
};

} // namespace (anonymous)

namespace storage {

std::vector<Book> loadBooksJson(const std::string& filePath, bool* ok) {
    if (ok) *ok = true;

    std::ifstream in(filePath);
    if (!in) return {}; // file doesn't exist yet -> empty is fine

    std::stringstream buffer;
    buffer << in.rdbuf();
    const std::string content = buffer.str();

    Parser p(content);
    std::vector<Book> books;
    if (!p.parseArray(books)) {
        if (ok) *ok = false;
        return {};
    }
    return books;
}

bool saveBooksJson(const std::string& filePath, const std::vector<Book>& books) {
    std::ofstream out(filePath, std::ios::trunc);
    if (!out) return false;

    out << "[\n";
    for (size_t i = 0; i < books.size(); i++) {
        const auto& b = books[i];
        out << "  {"
            << "\"id\":" << b.id << ","
            << "\"title\":\"" << jsonEscape(b.title) << "\","
            << "\"author\":\"" << jsonEscape(b.author) << "\","
            << "\"year\":" << b.year
            << "}";
        if (i + 1 < books.size()) out << ",";
        out << "\n";
    }
    out << "]\n";
    return true;
}

} // namespace storage
