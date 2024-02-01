#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void processFileUpload(const std::string& httpRequest)
{
    // Trouver la ligne de séparation entre les champs du formulaire
    size_t boundaryPos = httpRequest.find("boundary=");
    if (boundaryPos == std::string::npos) {
        std::cerr << "Boundary not found in the request\n";
        return;
    }
    std::string boundary = "--" + httpRequest.substr(boundaryPos + 9);

    // Trouver le début et la fin du champ de fichier dans la requête
    size_t fileStart = httpRequest.find(boundary);
    size_t fileEnd = httpRequest.find(boundary, fileStart + boundary.size());
    if (fileStart == std::string::npos || fileEnd == std::string::npos) {
        std::cerr << "File data not found in the request\n";
        return;
    }

    // Extraire le nom du fichier à partir de la requête
    size_t filenamePos = httpRequest.find("filename=\"", fileStart);
    if (filenamePos == std::string::npos || filenamePos >= fileEnd) {
        std::cerr << "Filename not found in the request\n";
        return;
    }
    size_t filenameStart = filenamePos + 10;
    size_t filenameEnd = httpRequest.find("\"", filenameStart);
    std::string filename = httpRequest.substr(filenameStart, filenameEnd - filenameStart);

    // Extraire les données du fichier à partir de la requête
    size_t fileDataStart = httpRequest.find("\r\n\r\n", fileStart);
    if (fileDataStart == std::string::npos || fileDataStart >= fileEnd) {
        std::cerr << "File data not found in the request\n";
        return;
    }
    fileDataStart += 4; // Passer les délimiteurs

    std::string fileData = httpRequest.substr(fileDataStart, fileEnd - fileDataStart);

    // Écrire les données du fichier dans un fichier sur le serveur ou traiter les données selon vos besoins
    std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Failed to open output file\n";
        return;
    }
    outputFile.write(fileData.c_str(), fileData.size());
    outputFile.close();

    std::cout << "File uploaded successfully: " << filename << "\n";
}
