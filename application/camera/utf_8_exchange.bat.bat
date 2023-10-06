@setlocal&set "a=%*"&powershell -nop "'&{#'+(gc \""%~f0\"" -raw)+'} '+($Env:a -replace '(?=[''(){},;`$&])','`')|iex"&pause&exit/b

# Shift_JIS -> UTF-8 (no BOM)

$ErrorActionPreference = 'Stop'
$fileTypes = @('*.h', '*.cpp')

function sjis_to_utf8($src, $dst) {
    $text = [IO.File]::ReadAllText($src, [Text.Encoding]::GetEncoding(932))
    New-Item -Path $dst -Value $text -ItemType File -Force > $null
}

foreach ($file in Get-ChildItem -Path $args) {
    if (Test-Path -LiteralPath $file -Include $fileTypes -Type Leaf) {
        $dst = $file.fullname + $file.extension
        echo $dst
        sjis_to_utf8 $file $dst
    }
}