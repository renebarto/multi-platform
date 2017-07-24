#include "xml/XMLParsingData.h"

#include "osal/Strings.h"
#include "xml/XMLBase.h"

using namespace XMLParser;

const char XMLParser::UTF_BOM0   = 0xef;
const char XMLParser::UTF_BOM1   = 0xbb;
const char XMLParser::UTF_BOM2   = 0xbf;
const char XMLParser::UTF_INV1_1 = 0xbf;
const char XMLParser::UTF_INV1_2 = 0xbe;
const char XMLParser::UTF_INV2_1 = 0xbf;
const char XMLParser::UTF_INV2_2 = 0xbf;
static const char CR             = '\r';
static const char LF             = '\n';
static const char TAB            = '\t';

void XMLParsingData::UpdateCursor(const XMLCursor & cursor)
{
    this->cursor = cursor;
}

void XMLParsingData::Update(const char * newString, XMLEncoding encoding)
{
    if (currentString == nullptr)
        return;

    int tabSize = XMLBase::tabSize;
    if (tabSize < 1)
    {
        tabSize = 1;
    }
    if (newString == nullptr)
        return;

    // Get the current row, column.
    int row = std::max(cursor.Row(), 0);
    int col = std::max(cursor.Col(), 0);

    while (currentString < newString)
    {
        switch (*currentString)
        {
        case 0:
            // We *should* never get here, but in case we do, don't
            // advance past the terminating null character, ever
            return;

        case CR:
            // Bump down to the next line
            ++row;
            col = 0;
            ++currentString;

            // Check for CRLF sequence, and treat this as a single character
            if (*currentString == LF)
            {
                ++currentString;
            }
            break;

        case LF:
            // bump down to the next line
            ++row;
            col = 0;

            // Eat the character
            ++currentString;

            // Check for LFCR sequence, and treat this as a single
            // character.
            if (*currentString == CR)
            {
                ++currentString;
            }
            break;

        case TAB:
            // Eat the character
            ++currentString;

            // Skip to next tab stop
            col = ((col - 1 + tabSize) / tabSize) * tabSize;
            break;

        case XMLParser::UTF_BOM0:
            if (encoding == XMLEncoding::UTF8)
            {
                if ((*(currentString + 1) != '\0') && (*(currentString + 2) != '\0'))
                {
                    // In these cases, don't advance the column. These are
                    // 0-width spaces.
                    if ((*(currentString + 1) == XMLParser::UTF_BOM1)
                        && (*(currentString + 2) == XMLParser::UTF_BOM2))
                        currentString += 3;
                    else if ((*(currentString + 1) == XMLParser::UTF_INV1_1)
                             && (*(currentString + 2) == XMLParser::UTF_INV1_2))
                        currentString += 3;
                    else if ((*(currentString + 1) == XMLParser::UTF_INV2_1)
                             && (*(currentString + 2) == XMLParser::UTF_INV2_2))
                        currentString += 3;
                    else // A normal character.
                    {
                        currentString += 3;
                        ++col;
                    }
                }
            }
            else
            {
                ++currentString;
                ++col;
            }
            break;

        default:
            if (encoding == XMLEncoding::UTF8)
            {
                // Eat the 1 to 4 byte utf8 character.
                int numBytes = XMLBase::UTF8ByteTable[(unsigned char)*currentString];
                if (numBytes == 0)
                    numBytes = 1; // Error case from bad encoding, but handle gracefully.
                currentString += numBytes;

                // Just advance one column, of course.
                ++col;
            }
            else
            {
                ++currentString;
                ++col;
            }
            break;
        }
    }
    cursor.Set(row, col);
    assert(cursor.Row() >= 0);
    assert(cursor.Col() >= 0);
}
