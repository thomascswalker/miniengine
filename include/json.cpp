#include "json.h"

namespace JSON
{
    std::string getIndent(int indent = 0)
    {
        auto size = indent * 4;
        auto indentString = std::string(size, ' ');
        return indentString;
    }

    std::string formatLine(const std::string& value, int indent, bool end)
    {
        std::string line;
        line += getIndent(indent);
        line += value;
        if (!end)
        {
            line += ",";
        }
        line += "\n";
        return line;
    }

    std::string formatLine(const std::string& key, const std::string& value,
        int indent, bool end)
    {
        std::string line;
        line += getIndent(indent);
        line += "\"" + key + "\"" + ": " + value;
        if (!end)
        {
            line += ",";
        }
        line += "\n";
        return line;
    }

// General operators
    std::ostream& operator<<(std::ostream& o, JsonArray& a)
    {
        return o << JsonObject(a).format();
    }

    std::ostream& operator<<(std::ostream& o, JsonDict& d)
    {
        return o << JsonObject(d).format();
    }

// NullType
    std::string NullValue::format()
    {
        return "NullType";
    }

    std::ostream& NullValue::operator<<(std::ostream& o)
    {
        return o << format();
    }

// Bool
    bool BoolValue::value() const
    {
        return m_value;
    }

    std::string BoolValue::format()
    {
        std::string boolString(m_value ? std::string("true") : std::string("false"));
#if DEBUG_TYPE == true
        boolString += " (bool)";
#endif
        return boolString;
    }

    std::ostream& BoolValue::operator<<(std::ostream& o)
    {
        return o << format();
    }

    BoolValue::BoolValue(const BoolValue& other)
    {
        *this = other;
    }

// Int
    int IntValue::value() const
    {
        return m_value;
    }

    std::string IntValue::format()
    {
        std::string intString = std::to_string(m_value);
#if DEBUG_TYPE == true
        intString += " (int)";
#endif
        return intString;
    }

    IntValue::IntValue(const IntValue& other)
    {
        *this = other;
    }

    std::ostream& IntValue::operator<<(std::ostream& o)
    {
        return o << format();
    }

// Double
    double DoubleValue::value() const
    {
        return m_value;
    }

    std::string DoubleValue::format()
    {
        std::string doubleString = std::to_string(m_value);
#if DEBUG_TYPE == true
        doubleString += " (double)";
#endif
        return doubleString;
    }

    DoubleValue::DoubleValue(const DoubleValue& other)
    {
        *this = other;
    }

    std::ostream& DoubleValue::operator<<(std::ostream& o)
    {
        return o << format();
    }

// StringType
    std::string StringValue::value()
    {
        return m_value;
    }

    std::string StringValue::format()
    {

        std::string string = "\"" + m_value + "\"";
#if DEBUG_TYPE == true
        string += " (string)";
#endif
        return string;
    }

    StringValue::StringValue(const StringValue& other)
    {
        *this = other;
    }

    std::ostream& StringValue::operator<<(std::ostream& o)
    {
        return o << format();
    }

// Array
    ArrayValue::ArrayValue(const JsonArray& value)
    {
        m_value.clear();
        for (const JsonObject& v : value)
        {
            m_value.emplace_back(v);
        }
    }

    JsonArray ArrayValue::value()
    {
        return m_value;
    }

    std::string ArrayValue::format()
    {
        std::string arrayString = "[\n";
        CURRENT_INDENT++;
        int count = 1;
        for (auto& v : m_value)
        {
            bool at_end = (count == m_value.size());
            arrayString += formatLine(v.format(), CURRENT_INDENT, at_end);
            count++;
        }
        CURRENT_INDENT--;
        arrayString += getIndent(CURRENT_INDENT) + "]";
        return arrayString;
    }

// Dictionary
    DictValue::DictValue(const JsonDict& value)
    {
        for (const auto& [k, v] : value)
        {
            m_value[k] = v;
        }
    }

    JsonDict DictValue::value()
    {
        return m_value;
    }

    std::string DictValue::format()
    {
        std::string dictString = "{\n";
        CURRENT_INDENT++;
        int count = 1;
        for (auto& [k, v] : m_value)
        {
            bool at_end = (count == m_value.size());
            std::string new_line = (v.type() == Dictionary || v.type() == Array)
                                   ? ("\n" + getIndent(CURRENT_INDENT))
                                   : "";
            dictString += formatLine(k, new_line + v.format(), CURRENT_INDENT, at_end);
            count++;
        }
        CURRENT_INDENT--;
        dictString += getIndent(CURRENT_INDENT) + "}";
        return dictString;
    }

    // JSON Object
    JsonObject::JsonObject()
    {
        m_value = nullptr;
        m_type = EValueType::Null;
    }

    JsonObject::JsonObject(const JsonObject& other)
    {
        *this = other;
        m_type = other.m_type;
    }

    JsonObject::JsonObject(bool value)
    {
        m_value = std::make_unique<BoolValue>(value);
        m_type = Bool;
    }

    JsonObject::JsonObject(int value)
    {
        m_value = std::make_unique<IntValue>(value);
        m_type = Int;
    }

    JsonObject::JsonObject(double value)
    {
        m_value = std::make_unique<DoubleValue>(value);
        m_type = Double;
    }

    JsonObject::JsonObject(const std::string& value)
    {
        m_value = std::make_unique<StringValue>(value);
        m_type = EValueType::String;
    }

    JsonObject::JsonObject(const JsonArray& value)
    {
        m_value = std::make_unique<ArrayValue>(value);
        m_type = Array;
    }

    JsonObject::JsonObject(const JsonDict& value)
    {
        m_value = std::make_unique<DictValue>(value);
        m_type = Dictionary;
    }

    BoolValue& JsonObject::asBool() const
    {
        return *dynamic_cast<BoolValue*>(m_value.get());
    }

    IntValue& JsonObject::asInt() const
    {
        return *dynamic_cast<IntValue*>(m_value.get());
    }

    DoubleValue& JsonObject::asDouble() const
    {
        return *dynamic_cast<DoubleValue*>(m_value.get());
    }

    StringValue& JsonObject::asString() const
    {
        return *dynamic_cast<StringValue*>(m_value.get());
    }

    ArrayValue& JsonObject::asArray() const
    {
        return *dynamic_cast<ArrayValue*>(m_value.get());
    }

    DictValue& JsonObject::asDict() const
    {
        return *dynamic_cast<DictValue*>(m_value.get());
    }

    bool JsonObject::getBool() const
    {
        return asBool().value();
    }

    int JsonObject::getInt() const
    {
        return asInt().value();
    }

    double JsonObject::getDouble() const
    {
        return asDouble().value();
    }

    std::string JsonObject::getString() const
    {
        return asString().value();
    }

    JsonArray JsonObject::getArray() const
    {
        return asArray().value();
    }

    JsonDict JsonObject::getDict() const
    {
        return asDict().value();
    }

    std::string JsonObject::format() const
    {
        if (m_value == nullptr)
        {
            return "NULL";
        }

        return m_value->format();
    }

    JsonObject& JsonObject::operator=(const JsonObject& other)
    {
        switch (other.m_type)
        {
        case (Bool):
        {
            m_value = std::make_unique<BoolValue>(other.getBool());
            break;
        }
        case (Int):
        {
            m_value = std::make_unique<IntValue>(other.getInt());
            break;
        }
        case (Double):
        {
            m_value = std::make_unique<DoubleValue>(other.getDouble());
            break;
        }
        case (String):
        {
            m_value = std::make_unique<StringValue>(other.getString());
            break;
        }
        case (Array):
        {
            m_value = std::make_unique<ArrayValue>(other.getArray());
            break;
        }
        case (Dictionary):
        {
            m_value = std::make_unique<DictValue>(other.getDict());
            break;
        }
        default:
        {
            break;
        }
        }
        m_type = other.m_type;
        return *this;
    }

    JsonObject& JsonObject::operator[](const std::string& key)
    {
        if (m_type != Dictionary && m_value == nullptr)
        {
            throw std::runtime_error("Invalid type, wanted Dictionary");
        }
        return asDict()[key];
    }

    JsonObject& JsonObject::operator[](int index)
    {
        if (m_type != Array)
        {
            throw std::runtime_error("Invalid type, wanted Array");
        }
        return asArray()[index];
    }

    std::ostream& operator<<(std::ostream& o, ArrayValue& a)
    {
        return o << a.format();
    }

    ArrayValue& ArrayValue::operator=([[maybe_unused]] const ArrayValue& other)
    {
        this->m_value = other.m_value;
        return *this;
    }

    JsonObject& ArrayValue::operator[]([[maybe_unused]] const int index)
    {
        if (index > m_value.size())
        {
            throw std::runtime_error("Index out of bounds.");
        }
        return m_value[index];
    }

    std::ostream& ArrayValue::operator<<(std::ostream& o)
    {
        return o << format();
    }

    ArrayValue::ArrayValue(const ArrayValue& other)
    {
        *this = other;
    }

    std::ostream& operator<<(std::ostream& o, DictValue& d)
    {
        return o << d.format();
    }

    DictValue::DictValue(const DictValue& other)
    {
        *this = other;
    }

    DictValue& DictValue::operator=([[maybe_unused]] const DictValue& other)
    {
        m_value = other.m_value;
        return *this;
    }

    JsonObject& DictValue::operator[](const std::string& key)
    {
        return m_value[key];
    }

    std::ostream& operator<<(std::ostream& o, JsonObject& j)
    {
        return o << j.format();
    }

    std::ostream& operator<<(std::ostream& o, const JsonObject& j)
    {
        return o << j.format();
    }
    size_t JsonObject::size()
    {
        if (m_type == Array)
        {
            auto a = asArray();
            return a.value().size();
        }
        if (m_type == Dictionary)
        {
            auto a = asDict();
            return a.value().size();
        }

        throw std::runtime_error("No size accessor for this JSON object type.");
    }

    JsonObject loadFile(const std::string& filename)
    {
        // Read file contents
        std::ifstream file(filename); // Loading file as input stream
        std::string data;
        if (file)
        {
            std::ostringstream stream; // New stream
            stream << file.rdbuf();    // Reading data
            data = stream.str();       // Put stream to data string
        }
        else
        {
            throw std::runtime_error("File not found: " + filename);
        }

        // Tokenize string
        auto* l = new Lexer(data);

        // Parse string into JSON object
        auto* p = new Parser(l);
        return p->get();
    }

    JsonObject loadString(std::string& string)
    {
        // Tokenize string
        auto* l = new Lexer(string);

        // Parse string into JSON object
        auto* p = new Parser(l);
        return p->get();
    }

    // Lexer
    std::string Lexer::sanitize(std::string& input)
    {
        std::string output;

        // Whether we're inside a string or not at the current character
        bool inString = false;

        // For every character in the input string...
        for (char& c : input)
        {
            // Remove new lines and end of lines
            if (c == '\n' || c == '\0')
            {
                continue;
            }

            // Remove spaces ONLY when outside a string
            if (c == ' ' && !inString)
            {
                continue;
            }

            // Flip-flop context of in or outside a string
            if (c == '"')
            {
                inString = !inString;
            }

            // Append current character to sanitized output
            output += c;
        }

        return output;
    }

    Lexer::Lexer(std::string string)
    {
        // Sanitize input string
        m_string = sanitize(string);

        // While we are able to continue, keep going to the next token
        while (canContinue())
        {
            // Get next token
            Token t = next();

            // TODO: Figure out why type is sometimes invalid.
            // This is a catch-all for invalid enum values.
            if (t.type < 0)
            {
                continue;
            }

            // Add to token array.
            tokens.push_back(t);
        }
    }

    bool Lexer::canContinue()
    {
        return m_offset < m_string.size();
    }

    Token Lexer::next()
    {
        Token token;

        // Numbers
        if (IS_NUMBER(m_string[m_offset]))
        {
            std::string number;
            while (IS_NUMBER(m_string[m_offset]))
            {
                number += m_string[m_offset];
                m_offset++;
            }

            token.type = EValueType::Number;
            token.value = number;
            return token;
        }

        // Strings
        if (IS_QUOTE(m_string[m_offset]))
        {
            std::string string;

            // Skip entry quote
            m_offset++;

            // Build the string from contents inside the quotes
            while (IS_NOT_QUOTE(m_string[m_offset]))
            {
                string += m_string[m_offset];
                m_offset++;
            }

            // Skip exit quote
            m_offset++;
            token.type = EValueType::String;
            token.value = string;
            return token;
        }

        // Booleans
        if (m_string.substr(m_offset, 4) == "true")
        {
            m_offset += 4;
            token.type = EValueType::Bool;
            token.value = "true";
            return token;
        }
        if (m_string.substr(m_offset, 5) == "false")
        {
            m_offset += 5;
            token.type = EValueType::Bool;
            token.value = "false";
            return token;
        }

        // Null
        if (m_string.substr(m_offset, 4) == "null")
        {
            m_offset += 4;
            token.type = EValueType::Null;
            return token;
        }

        // Separators
        if (IS_COMMA(m_string[m_offset]))
        {
            m_offset++;
            token.type = EValueType::Comma;
            return token;
        }

        if (IS_LBRACE(m_string[m_offset]))
        {
            m_offset++;
            token.type = EValueType::LBrace;
            return token;
        }

        if (IS_RBRACE(m_string[m_offset]))
        {
            m_offset++;
            token.type = EValueType::RBrace;
            return token;
        }

        if (IS_LBRACKET(m_string[m_offset]))
        {
            m_offset++;
            token.type = EValueType::LBracket;
            return token;
        }

        if (IS_RBRACKET(m_string[m_offset]))
        {
            m_offset++;
            token.type = EValueType::RBracket;
            return token;
        }

        if (IS_COLON(m_string[m_offset]))
        {
            m_offset++;
            token.type = EValueType::Colon;
            return token;
        }

        // In all other instances, we have a malformed JSON file. Throw an error.
        std::string msg(&"Invalid character "[m_string[m_offset]]);
        throw std::runtime_error(msg);
    }

    // Parser
    void Parser::next()
    {
        current++;
        pos++;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

    JsonObject Parser::parse()
    {
        // NullType
        switch (current->type)
        {
        case (EValueType::Null):
        {
            next(); // Go to next token
            return {};
        }

            // Booleans
        case (EValueType::Bool):
        {
            std::string value = current->value;
            next(); // Go to next token
            return (value == "true" ? JsonObject(true) : JsonObject(false));
        }

            // Numbers
        case (EValueType::Number):
        {
            std::string value = current->value;
            next(); // Go to next token
            // Decimal values
            if (value.find('.') != std::string::npos)
            {
                return JsonObject(std::stod(value));
            }
                // Integer values
            else
            {
                return JsonObject(std::stoi(value));
            }
        }

            // Strings
        case (EValueType::String):
        {
            std::string value = current->value;
            next(); // Go to next token
            return JsonObject(value);
        }

            // Arrays
        case (EValueType::LBrace):
        {
            next(); // Skip start brace
            JsonArray array;
            while (current->type != EValueType::RBrace)
            {
                // Skip commas
                if (current->type == EValueType::Comma)
                {
                    next(); // Go to next token
                    continue;
                }
                JsonObject value = parse(); // Recursively parse value

                // TODO: Figure out why this is needed, otherwise it breaks
                if (value.type() == EValueType::Null)
                {
                    break;
                }

                // Add to our array the value we parsed
                array.push_back(value);
            }

            next(); // Skip end brace
            return JsonObject(array);
        }

            // Dictionaries
        case (EValueType::LBracket):
        {
            next(); // Skip start bracket
            JsonDict dict;

            while (current->type != EValueType::RBracket && current->type > 0)
            {
                // Parse key
                if (current->type != EValueType::String)
                {
                    throw std::runtime_error("Expected string key");
                }
                std::string key = current->value;
                next(); // Move from key to expected colon

                // Parse value
                if (current->type != EValueType::Colon)
                {
                    throw std::runtime_error("Expected colon");
                }
                next(); // Move from colon to expected value

                // Construct dict obj
                JsonObject value = parse(); // Recursively parse value
                dict[key] = value;

                // If there's a comma, skip it
                if (current->type == EValueType::Comma)
                {
                    next();
                    continue;
                }
                // If we're at the end of the dictionary, break the loop
                if (current->type == EValueType::RBracket)
                {
                    break;
                }
            }

            next(); // Skip end bracket
            return JsonObject(dict);
        }

        default:
        {
            throw std::runtime_error("Unable to parse!");
        }
        }
    }

#pragma clang diagnostic pop

    Parser::Parser(Lexer* lexer) : m_lexer(lexer)
    {
        current = &m_lexer->tokens[0];
        m_json = parse();
    }

    JsonObject& Parser::get()
    {
        return m_json;
    }
} // namespace JSON
