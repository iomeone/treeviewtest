
#include "jucer_FileHelpers.h"

namespace FileHelpers
{


	String unixStylePath(const String& path) { return path.replaceCharacter('\\', '/'); }
	String windowsStylePath(const String& path) { return path.replaceCharacter('/', '\\'); }

	String currentOSStylePath(const String& path)
	{
#if JUCE_WINDOWS
		return windowsStylePath(path);
#else
		return unixStylePath(path);
#endif
	}



	bool isAbsolutePath(const String& path)
	{
		return File::isAbsolutePath(path)
			|| path.startsWithChar('/') // (needed because File::isAbsolutePath will ignore forward-slashes on Windows)
			|| path.startsWithChar('$')
			|| path.startsWithChar('~')
			|| (CharacterFunctions::isLetter(path[0]) && path[1] == ':')
			|| path.startsWithIgnoreCase("smb:");
	}


	String getRelativePathFrom(const File& file, const File& sourceFolder)
	{
#if ! JUCE_WINDOWS
		// On a non-windows machine, we can't know if a drive-letter path may be relative or not.
		if (CharacterFunctions::isLetter(file.getFullPathName()[0]) && file.getFullPathName()[1] == ':')
			return file.getFullPathName();
#endif

		return file.getRelativePathFrom(sourceFolder);
	}



	
}