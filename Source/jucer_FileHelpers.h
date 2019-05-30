#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
namespace FileHelpers
{
	//uint64 calculateStreamHashCode(InputStream&);
	//uint64 calculateFileHashCode(const File&);

	//bool overwriteFileWithNewDataIfDifferent(const File& file, const void* data, size_t numBytes);
	//bool overwriteFileWithNewDataIfDifferent(const File& file, const MemoryOutputStream& newData);
	//bool overwriteFileWithNewDataIfDifferent(const File& file, const String& newData);

	//bool containsAnyNonHiddenFiles(const File& folder);

	String unixStylePath(const String& path);
	String windowsStylePath(const String& path);
	String currentOSStylePath(const String& path);

	//bool shouldPathsBeRelative(String path1, String path2);
	bool isAbsolutePath(const String& path);

	//// A windows-aware version of File::getRelativePath()
	String getRelativePathFrom(const File& file, const File& sourceFolder);

	//// removes "/../" bits from the middle of the path
	//String simplifyPath(String::CharPointerType path);
	//String simplifyPath(const String& path);
}