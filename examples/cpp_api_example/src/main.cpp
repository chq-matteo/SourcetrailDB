#include <cstdlib>
#include <fstream>
#include <iostream>

#include "SourcetrailDBWriter.h"

int main(int argc, const char *argv[])
{
	sourcetrail::SourcetrailDBWriter dbWriter;

	std::cout << "\nSourcetrailDB C++ API Example" << std::endl;
	std::cout << std::endl;
	std::cout << "SourcetrailDB version: " << dbWriter.getVersionString() << std::endl;
	std::cout << "Supported database version: " << dbWriter.getSupportedDatabaseVersion() << std::endl;
	std::cout << std::endl;

	if (argc < 3 || argc > 4)
	{
		std::cout << "usage: cpp_api_example <database_path> <source_path> <optional:database_version>";
	}

	std::string dbPath = argv[1];
	std::string sourcePath = argv[2];

	int dbVersion = 0;
	if (argc == 4)
	{
		char* end;
		dbVersion = strtol(argv[3], &end, 10);
	}

	if (dbVersion && dbVersion != dbWriter.getSupportedDatabaseVersion())
	{
		std::cerr << "error: binary only supports database version: " << dbWriter.getSupportedDatabaseVersion()
			<< ". Requested version: " << dbVersion << std::endl;
		return 1;
	}

	// open database by passing .srctrldb or .srctrldb_tmp path
	std::cout << "Opening Database: " << dbPath << std::endl;
	if (!dbWriter.open(dbPath))
	{
		std::cerr << "error: " << dbWriter.getLastError() << std::endl;
		return 1;
	}

	std::cout << "Starting Indexing..." << std::endl;

	// start recording with faster speed
	if (!dbWriter.beginTransaction())
	{
		std::cerr << "error: " << dbWriter.getLastError() << std::endl;
		return 1;
	}

	// record source file by passing it's absolute path
	int fileId = dbWriter.recordFile(sourcePath);
	dbWriter.recordFileLanguage(fileId, "cpp"); // record file language for syntax highlighting


	// record comment
	dbWriter.recordCommentLocation({ fileId, 2, 1, 6, 3 });


	// record namespace "api"
	sourcetrail::NameHierarchy namespaceName { "::", { { "", "api", "" } } };
	int namespaceId = dbWriter.recordSymbol(namespaceName);
	dbWriter.recordSymbolDefinitionKind(namespaceId, sourcetrail::DefinitionKind::EXPLICIT);
	dbWriter.recordSymbolKind(namespaceId, sourcetrail::SymbolKind::NAMESPACE);
	dbWriter.recordSymbolLocation(namespaceId, { fileId, 8, 11, 8, 13 });
	dbWriter.recordSymbolScopeLocation(namespaceId, { fileId, 8, 1, 24, 1 });


	// record class "MyType"
	sourcetrail::NameHierarchy className = namespaceName;
	className.nameElements.push_back( { "", "MyType", "" } );
	int classId = dbWriter.recordSymbol(className);
	dbWriter.recordSymbolDefinitionKind(classId, sourcetrail::DefinitionKind::EXPLICIT);
	dbWriter.recordSymbolKind(classId, sourcetrail::SymbolKind::CLASS);
	dbWriter.recordSymbolLocation(classId, { fileId, 11, 7, 11, 12 });
	dbWriter.recordSymbolScopeLocation(classId, { fileId, 11, 1, 22, 1 }); // gets highlight when active


	// record inheritance reference to "BaseType"
	int baseId = dbWriter.recordSymbol( { "::", { { "", "BaseType", "" } } } );
	int inheritanceId = dbWriter.recordReference(baseId, classId, sourcetrail::ReferenceKind::INHERITANCE);
	dbWriter.recordReferenceLocation(inheritanceId, { fileId, 12, 14, 12, 21 });


	// add child method "void my_method() const"
	sourcetrail::NameHierarchy methodName = className;
	methodName.nameElements.push_back( { "void", "my_method", "() const" } );
	int methodId = dbWriter.recordSymbol(methodName);
	dbWriter.recordSymbolDefinitionKind(methodId, sourcetrail::DefinitionKind::EXPLICIT);
	dbWriter.recordSymbolKind(methodId, sourcetrail::SymbolKind::METHOD);
	dbWriter.recordSymbolLocation(methodId, { fileId, 15, 10, 15, 18 });
	dbWriter.recordSymbolScopeLocation(methodId, { fileId, 15, 5, 21, 5 }); // gets highlight when active
	dbWriter.recordSymbolSignatureLocation(methodId, { fileId, 15, 5, 15, 45 }); // used in tooltip


	// record parameter type "bool"
	int typeId = dbWriter.recordSymbol({ "::", { { "", "bool", "" } } });
	int typeuseId = dbWriter.recordReference(methodId, typeId, sourcetrail::ReferenceKind::TYPE_USAGE);
	dbWriter.recordReferenceLocation(typeuseId, { fileId, 15, 20, 15, 23 });


	// record parameter "do_send_signal"
	int localId = dbWriter.recordLocalSymbol("do_send_signal");
	dbWriter.recordLocalSymbolLocation(localId, { fileId, 15, 25, 15, 38 });
	dbWriter.recordLocalSymbolLocation(localId, { fileId, 17, 13, 17, 26 });


	// record function call reference to "send_signal()"
	int funcId = dbWriter.recordSymbol({ "::", { { "", "Client", "" }, { "", "send_signal", "()" } } });
	dbWriter.recordSymbolKind(funcId, sourcetrail::SymbolKind::FUNCTION);
	int callId = dbWriter.recordReference(methodId, funcId, sourcetrail::ReferenceKind::CALL);
	dbWriter.recordReferenceLocation(callId, { fileId, 19, 13, 19, 33 });


	// record error
	dbWriter.recordError("Really? You missed that \";\" again? (intentional error)", false, { fileId, 22, 1, 22, 1 });


	// end recording
	if (!dbWriter.commitTransaction())
	{
		std::cerr << "error: " << dbWriter.getLastError() << std::endl;
		return 1;
	}

	// check for errors before finishing
	if (dbWriter.getLastError().size())
	{
		std::cerr << "error: " << dbWriter.getLastError() << std::endl;
		return 1;
	}

	std::cout << "done!" << std::endl;

	return 0;
}
