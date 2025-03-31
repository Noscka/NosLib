$HEADER = "AllHeaders.hpp"

If (Test-Path $HEADER)
{
    Remove-Item $HEADER
}

"#ifndef __ALL_HEADERS__" >> $HEADER
"#define __ALL_HEADERS__" >> $HEADER

$HeaderPath = "../NosLib/Include/NosLib/"
$headerBase = (Resolve-Path $HeaderPath).Path

Get-ChildItem -Path $HeaderPath -Recurse -File |
    Where-Object { $_.Extension -eq ".h" -or $_.Extension -eq ".hpp" } |
    ForEach-Object {
        $relativePath = $_.FullName.Substring($headerBase.Length)
        if ($relativePath.StartsWith("\") -or $relativePath.StartsWith("/")) {
            $relativePath = $relativePath.Substring(1)
        }
        $relativePath = $relativePath -replace '\\', '/'
        "#include <NosLib/$relativePath>" >> $HEADER
    }

"#endif" >> $HEADER
