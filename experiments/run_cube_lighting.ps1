param(
    [string]$BuildDirectory = 'out/build/x64-Debug',
    [string]$Texture = 'assets/texture/img1.jpg',
    [string]$OutputDirectory = 'out/cube-lighting-experiment'
)

$ErrorActionPreference = 'Stop'
$repositoryRoot = Split-Path -Parent $PSScriptRoot
Push-Location -LiteralPath $repositoryRoot
try {
    $executable = Join-Path $BuildDirectory 'experiments/cubeLightingExperiment.exe'
    if (!(Test-Path -LiteralPath $executable)) {
        throw 'Build the cubeLightingExperiment CMake target first.'
    }
    & $executable $Texture $OutputDirectory
    if ($LASTEXITCODE -ne 0) {
        throw "Experiment failed with exit code $LASTEXITCODE"
    }

    # Lossless format conversion of the actual framebuffer exports.
    Add-Type -AssemblyName System.Drawing
    foreach ($name in @('cube_light_actual', 'cube_texture_control', 'cube_texture_frame2', 'cube_main_scene')) {
        $bmpPath = Join-Path $OutputDirectory ($name + '.bmp')
        $pngPath = Join-Path $OutputDirectory ($name + '.png')
        $bitmap = [System.Drawing.Image]::FromFile((Resolve-Path -LiteralPath $bmpPath).Path)
        try {
            $bitmap.Save([System.IO.Path]::GetFullPath($pngPath), [System.Drawing.Imaging.ImageFormat]::Png)
        } finally {
            $bitmap.Dispose()
        }
    }
} finally {
    Pop-Location
}
