#ifndef STRINGALGORITHM_H
#define STRINGALGORITHM_H

// STL
#include <algorithm>
#include <map>
#include <sstream>

/**
Verifies if a string ends with the string in parameter.
\param str String which will be verified.
\param endStr String that should be the end of str.
\return true if the string ends with endStr; otherwise returns false.
*/
template <typename T>
bool endsWith(const std::basic_string<T>& str, const std::basic_string<T>& endStr);

/**
* Join function.
* \param[in] elems Strings to join.
* \param[in] glue Caracter which will be put between all strings in elems.
* \param[out] s String which will be contains the final joined string.
*                     This parameter is also returned by the function.
* \return The s parameter is returned to allow some flexibility.
*/
template <typename T> std::basic_string<T>& join(const std::vector<std::basic_string<T> > &elems,
	char glue,
	std::basic_string<T>& s);

/**
* Join function.
* \param[in] elems Strings to join.
* \param[in] glue Caracter which will be put between all strings in elems.
* \return The final joined string.
*/
template <typename T> std::basic_string<T> join(const std::vector<std::basic_string<T> > &elems, char glue);

/**
* Split function.
* \param[in] s String to split.
* \param[in] delim Caracter where the string will be splitted.
* \param[out] elems A std::vector that will be fill with all substrings that will be found.
*                     This parameter is also returned by the function.
* \param[in] skipEmpty Determines if empty substrings found must be added to the \c elems
*                     vector or not. (false (default) adds them, true doesn't add them).
* \return The elems parameter is returned to allow some flexibility.
*/
template <typename T> std::vector<std::basic_string<T> >& split(const std::basic_string<T>& s,
	char delim,
	std::vector<std::basic_string<T> > &elems,
	bool skipEmpty=false);

/**
* Split function.
* \param[in] s String to split.
* \param[in] delim Caracter where the string will be splitted.
* \param[in] skipEmpty Determines if empty substrings found must be added to the \c elems
*                     vector or not. (false (default) adds them, true doesn't add them).
* \return A std::vector of all substrings that were found.
*/
template <typename T>
std::vector<std::basic_string<T> > split(const std::basic_string<T>& s, char delim, bool skipEmpty=false);

/**
Converts everything which have a stream << operator to a std::basic_string<T>.
Especially usefull to convert numbers.
*/
template<typename T, typename U> inline std::basic_string<T> toString(const U& n);

/**
* Trim from start of a string
*/
template <typename T> inline std::basic_string<T>& ltrim(std::basic_string<T>& s);

/**
* Trim from end of a string
*/
template <typename T> inline std::basic_string<T>& rtrim(std::basic_string<T>& s);

/**
* Trim from both start and end of a string
*/
template <typename T> inline std::basic_string<T>& trim(std::basic_string<T>& s);

/**
Wordwrap function. Goes to a new line before reach \c width caracters.
Keeps the new-line symbols that were already present.
*/
template <typename T> std::basic_string<T>& wordwrap(std::basic_string<T>& str, int width=70);


/**
* \brief Replaces successive blanks in the string by a single space.
*/
template <typename T> std::basic_string<T>& replaceMultipleBlanksBySingleSpace(std::basic_string<T>& str);

/**
* \brief Replaces special sequences of characters in the string by their meaning.
*
* The two characters "\\" are replaced by a single '\',
* "\n" (the two characters '\'+'n') are replaced by '\n',
* "\t" (the two characters '\'+'t') are replaced by '\t'
*/
template <typename T> std::basic_string<T>& restoreSpecialChars(std::basic_string<T>& str);

/**
* \brief Transforms a text extracted from xml to a displayable text.
*
* For example, using this xml content:
* \code{.xml}
* <tag attrib="Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec
*              diam lectus. Sed sit amet ipsum mauris.\n Maecenas
*              congue ligula ac viverra nec consectetur ante hendrerit.">
* \endcode
* The following text will be extracted:
* \code
* Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec
*              diam lectus. Sed sit amet ipsum mauris.\n Maecenas
*              congue ligula ac viverra nec consectetur ante hendrerit.
* \endcode
* Using this function, you will have this:
* \code
* Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec diam lectus. Sed sit amet ipsum mauris.
* Maecenas congue ligula ac viverra nec consectetur ante hendrerit.
* \endcode
*/
template <typename T>
std::basic_string<T>& textFromXmlToDisplayableText(std::basic_string<T>& str);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
bool endsWith(const std::basic_string<T>& str, const std::basic_string<T>& endStr)
{
	int strSize = str.size();
	int endStrSize = endStr.size();

	if (strSize < endStrSize)
		return false;

	typename std::basic_string<T>::const_reverse_iterator itStr = str.crbegin();
	typename std::basic_string<T>::const_reverse_iterator itEndStr = endStr.crbegin();
	while (itEndStr != endStr.crend()) {
		if (*itStr != *itEndStr)
			return false;
		++itStr;
		++itEndStr;
	}
	return true;
}


template <typename T>
std::basic_string<T>& join(const std::vector<std::basic_string<T> > &elems, char glue, std::basic_string<T>& s)
{
	s.clear();
	typename std::vector<std::basic_string<T> >::const_iterator it = elems.cbegin();
	if (it == elems.cend())
		return s;

	s = *it;
	while (++it != elems.cend())
		s += (T)glue + *it;
	return s;
}

template <typename T>
std::basic_string<T> join(const std::vector<std::basic_string<T> > &elems, char glue)
{
	std::basic_string<T> s;
	return join(elems, glue, s);
}


template <typename T>
std::vector<std::basic_string<T> >& split(const std::basic_string<T>& s,
	char delim,
	std::vector<std::basic_string<T> > &elems,
	bool skipEmpty)
{
	std::basic_stringstream<T> ss(s);
	std::basic_string<T> item;
	while(std::getline(ss, item, (T)delim))
		if (!skipEmpty || !item.empty())
			elems.push_back(item);
	return elems;
}

template <typename T>
std::vector<std::basic_string<T> > split(const std::basic_string<T>& s, char delim, bool skipEmpty)
{
	std::vector<std::basic_string<T> > elems;
	return split(s, delim, elems, skipEmpty);
}


template<typename T, typename U>
inline std::basic_string<T> toString(const U& n)
{
	std::ostringstream oss;
	oss<<n;
	return oss.str();
}


template <typename T>
inline std::basic_string<T>& ltrim(std::basic_string<T>& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

template <typename T>
inline std::basic_string<T>& rtrim(std::basic_string<T>& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

template <typename T>
inline std::basic_string<T>& trim(std::basic_string<T>& s)
{
	return ltrim(rtrim(s));
}


template <typename T>
std::basic_string<T>& wordwrap(std::basic_string<T>& str, int width)
{
	typename std::basic_string<T>::iterator it;
	typename std::basic_string<T>::iterator lastBlank = str.begin();
	typename std::basic_string<T>::iterator lastWarp = str.begin();

	int distanceToWidth = 0;

	for (it = str.begin(); it != str.end(); ++it)
	{
		if (*it == (T)' ')
			lastBlank = it;
		else if (*it == (T)'\n') {
			lastWarp = lastBlank = it;
			distanceToWidth=0;
		}

		if (distanceToWidth == width) {
			if (lastBlank != lastWarp) {
				*lastBlank = (T)'\n';
				lastWarp = lastBlank;
				it = lastWarp;
			}
			else {
				str = str.substr(it-str.begin(), distanceToWidth) + (T)'\n'
					+ str.substr( (it-str.begin()) + distanceToWidth, std::basic_string<T>::npos);
				lastWarp = lastWarp + distanceToWidth - 1;
			}
			distanceToWidth=0;
		}
		distanceToWidth++;
	}

	return str;
}


template <typename T>
std::basic_string<T>& replaceMultipleBlanksBySingleSpace(std::basic_string<T>& str)
{
	if (str.begin() == str.end())
		return str;

	typename std::basic_string<T>::iterator it1 = ++str.begin();
	typename std::basic_string<T>::iterator it2 = str.begin();

	while (it1 != str.end())
	{
		if (isspace(*it1) && isspace(*it2)) {
			it1 = str.erase(it1);
		}
		else if (isspace(*it1)) {
			*it1 = (T)' ';
			++it1, ++it2;
		}
		else {
			++it1, ++it2;
		}
	}

	return str;
}

template <typename T>
std::basic_string<T>& restoreSpecialChars(std::basic_string<T>& str)
{
	if (str.begin() == str.end())
		return str;

	const static std::map<T, T> specialChars { std::make_pair((T)'\\', (T)'\\'),
		std::make_pair((T)'n',  (T)'\n'),
		std::make_pair((T)'t',  (T)'\t')
	};

	typename std::basic_string<T>::iterator it1 = ++str.begin();
	typename std::basic_string<T>::iterator it2 = str.begin();

	while (it1 != str.end())
	{
		if (*it2 == (T)'\\' && specialChars.find(*it1) != specialChars.cend()) {
			*it2 = specialChars.at(*it1);
			it1 = str.erase(it1);
		}
		else {
			++it1, ++it2;
		}
	}

	return str;
}

template <typename T>
std::basic_string<T>& textFromXmlToDisplayableText(std::basic_string<T>& str)
{
	replaceMultipleBlanksBySingleSpace(str);
	restoreSpecialChars(str);

	// Remove additionnal spaces just before or just after new lines.
	auto strLines = split(str, '\n');
	std::for_each(strLines.begin(), strLines.end(), trim<wchar_t>);
	join(strLines, '\n', str);

	return str;
}


#endif // STRINGALGORITHM_H