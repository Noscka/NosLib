$HEADER = "AllHeaders.hpp"

If (test-path $HEADER)
{
    Remove-Item $HEADER
}

"#ifndef __ALL_HEADERS__" >> $HEADER
"#define __ALL_HEADERS__" >> $HEADER
$files = Get-ChildItem "..\NosLib\NosLib\" -Filter *.hpp
foreach ($f in $files)
{
    "#include <NosLib\$f>" >> $HEADER
}
"#endif" >> $HEADER