#include "filereader.hh"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using json = nlohmann::json;

int Filereader::get_doc_start(std::string name) {
    // TODO
    return HEADER_LEN;
}

void Filereader::parse_fail() {
    std::cerr << "Could not parse file.\n";
    fclose(file);
    file = NULL;
    num_docs = 0;
    header_len = 1;
}

void Filereader::read_json(jsoninfo *js) {
    if (js -> j != nullptr) {
        std::cout << "Warning: Destination json already exists.\n";
        delete js -> j;
    }
    fseek(file, js -> doc_start, SEEK_SET);
    // Read the size of the json file
    int len;
    if (fread(&len, sizeof(int), 1, file) != 1) {
        parse_fail();
    }
    // Read the file
    char *jdata = (char *)malloc(len);
    if (fgets(jdata, len, file) == NULL) {
        parse_fail();
    }
    // Convert to string to json
    std::string s(jdata);
    js -> j = new json;
    *(js -> j) = json::parse(s);
}

void Filereader::write(jsoninfo *js) {
    std::stringstream output;
    output << *(js -> j) << std::endl;
    int len = output.str().length();
    fseek(file, get_doc_start((*(js -> j))["__name"]), SEEK_SET);
    fwrite(&len, sizeof(int), 1, file);
    fputs(output.str().c_str(), file);
}

// Find the first document with this name.
jsoninfo *Filereader::find_by_name(std::string name) {
    for (std::vector<jsoninfo>::iterator it = documents.begin(); it != documents.end(); ++it) {
        if (it -> name == name) {
            // Convert iterator to pointer
            return &(*it);
        }
    }
    return nullptr;
}

void Filereader::create(std::string name) {
    // Require name to be unique
    if (find_by_name(name) != nullptr) {
        std::cerr << "A document with that name already exists.\n";
        return;
    }
    json *j = new json;
    documents.push_back({j, 0, name});
    (*j)["__name"] = name;

    write(&documents.back());
}

void Filereader::edit(std::string name, std::string property, std::string value) {
    // Find the json with that name
    jsoninfo *js = find_by_name(name);
    if (!js) {
        std::cerr << "Name not recognized: " << name << "\n";
    }
    else {
        try {
            (*(js -> j))[property] = json::parse(value);
        }
        catch (nlohmann::detail::parse_error &e) {
            std::cerr << "Could not parse:\n    " << value << "\nError:\n" 
                      << e.what() << "\n";
        }
        write(js);
    }
}

void Filereader::view(std::string name, std::string property) {
    // Find the json with that name
    json *j = find_by_name(name) -> j;
    if (!j) {
        std::cerr << "Name not recognized: " << name << "\n";
    }
    else {
        std::cout << (*j)[property] << "\n";
    }
}

void Filereader::del(std::string name) {
    std::cerr << "DELETE not implemented.\n";
}

void Filereader::open(std::string name) {
    // mode r+, read and write
    file = fopen(name.c_str(), "r+");
    if (file == NULL) {
        // Create file if needed
        file = fopen(name.c_str(), "w");
        if (file == NULL) {
            std::cerr << "Error: unable to open file:\n    " << name << "\n";
            return;
        }
        num_docs = 0;
        fputc(header_len, file); // 1 (x HEADER_LEN)
        fputc(num_docs, file); // 0
        return;
    }
    header_len = (int)fgetc(file);
    num_docs = (int)fgetc(file);
    for (int i = 0; i < num_docs; ++i) {
        // Read int for file position
        int pos;
        if (fread(&pos, sizeof(int), 1, file) != 1) {
            parse_fail();
        }
        // Read char for name length
        int name_len = (int)fgetc(file);
        // Read name
        char *name = (char *)malloc(name_len);
        if (fgets(name, name_len, file) == NULL) {
            parse_fail();
        }
        documents.push_back({nullptr, pos, std::string(name)});
        free(name);
    }
}
