$HEADER = "AllHeaders.hpp"

If (test-path $HEADER)
{
    Remove-Item $HEADER
}

"#ifndef __ALL_HEADERS__" >> $HEADER
"#define __ALL_HEADERS__" >> $HEADER
$files = Get-ChildItem "..\NosStdLib\NosStdLib\" -Filter *.hpp
foreach ($f in $files)
{
    "#include <NosStdLib\$f>" >> $HEADER
}
"#endif" >> $HEADER