
1 
0
a

R1R2
R1|R2
R*
R?
(R)

Regex := Alt ("|" Regex)? 
Alt := Concat Alt?
Concat := R ["?""*"]?
R := "1" | "0" | "a" | "\?" | "(" Regex ")\\name"

struct Node {
	Node* left;
	Node* right;
	char op;
};

//  return root
std::pair<Node*,char*> parse_regex(char* start) {
	auto [left, s] = parse_alt(start);
	// Check
	if *s = '|' {
		s++;
		auto [right, s2] = parse_regex(start);

		return {new Node(left, right, '|'), s2}
	} else return {left, s};
}

std::pair<Node*, char*> parse_alt(char* start) {
    auto [left, s] = parse_concat(start);
    
    while (*s == '_') {
        s++;  // Move past the concatenation symbol
        auto [right, s2] = parse_concat(s);
        left = new Node(left, right, '_');
        s = s2;
    }

    return {left, s};
}

std::pair<Node*, char*> parse_concat(char* start) {
    auto [left, s] = parse_R(start);
    if (*s == '*' || *s == '?') {
        char op = *s;
        s++;
        return {new Node(left, nullptr, op), s};
    } else return {left, s};
}


std::pair<Node*, char*> parse_R(char* start) {
    char* s = start;
    char c = *s;
    
    // Parse any printable ASCII character
    if (std::isprint(c) && c != '|' && c != '*' && c != '?' && c != '(' && c != ')' && c != '_') {
        s++; // Move past the parsed character
        return {new Node(nullptr, nullptr, c), s};
    } else if (c == '(') {
        // Parse expression within parentheses
        s++; // Skip '('
        auto [expr, s2] = parse_regex(s);
        if (*s2 != ')') {
            // Handle error for missing closing parenthesis
        } else {
            s = s2 + 1; // Skip ')'
            return {expr, s};
        }
    } else {
        // Handle error for invalid character
    }
}



