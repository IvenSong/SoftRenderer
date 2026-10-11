$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
$outputDir = Join-Path (Split-Path -Parent $PSScriptRoot) 'out/blender-house-experiment'
$actual = [System.Drawing.Bitmap]::new((Join-Path $outputDir 'soft_renderer.bmp'))
$expected = [System.Drawing.Bitmap]::new((Join-Path $outputDir 'blender_reference.png'))
$flipped = [System.Drawing.Bitmap]::new((Join-Path $outputDir 'blender_v_flipped.png'))
try {
    $actual.Save((Join-Path $outputDir 'soft_renderer.png'), [System.Drawing.Imaging.ImageFormat]::Png)
    $intersection = 0L
    $union = 0L
    $normalError = 0L
    $flippedError = 0L
    for ($y = 0; $y -lt $actual.Height; ++$y) {
        for ($x = 0; $x -lt $actual.Width; ++$x) {
            $a = $actual.GetPixel($x, $y)
            $b = $expected.GetPixel($x, $y)
            $c = $flipped.GetPixel($x, $y)
            $maskA = ([int]$a.R + $a.G + $a.B) -gt 10
            $maskB = ([int]$b.R + $b.G + $b.B) -gt 10
            if ($maskA -or $maskB) { ++$union }
            if ($maskA -and $maskB) {
                ++$intersection
                $normalError += [Math]::Abs([int]$a.R-$b.R) + [Math]::Abs([int]$a.G-$b.G) + [Math]::Abs([int]$a.B-$b.B)
                $flippedError += [Math]::Abs([int]$a.R-$c.R) + [Math]::Abs([int]$a.G-$c.G) + [Math]::Abs([int]$a.B-$c.B)
            }
        }
    }
    $metrics = [ordered]@{
        silhouette_iou = [Math]::Round($intersection / [double]$union, 6)
        rgb_mae_original_reference = [Math]::Round($normalError / (3.0*$intersection), 4)
        rgb_mae_v_flipped_reference = [Math]::Round($flippedError / (3.0*$intersection), 4)
        rgb_range = '0..255; MAE evaluated on shared foreground pixels'
        note = 'Rasterization/antialiasing differ; this is a diagnostic comparison, not pixel identity.'
    }
    $metrics | ConvertTo-Json | Set-Content (Join-Path $outputDir 'comparison.json') -Encoding utf8
    $metrics | ConvertTo-Json

    $comparison = [System.Drawing.Bitmap]::new(1280, 524)
    $graphics = [System.Drawing.Graphics]::FromImage($comparison)
    $font = [System.Drawing.Font]::new('Arial', 18)
    try {
        $graphics.Clear([System.Drawing.Color]::FromArgb(24, 24, 24))
        $graphics.DrawString('Blender reference', $font, [System.Drawing.Brushes]::White, 18, 10)
        $graphics.DrawString('SoftRenderer - current code', $font, [System.Drawing.Brushes]::White, 658, 10)
        $graphics.DrawImage($expected, [System.Drawing.Rectangle]::new(0, 44, 640, 480), 0, 0, 640, 480, [System.Drawing.GraphicsUnit]::Pixel)
        $graphics.DrawImage($actual, [System.Drawing.Rectangle]::new(640, 44, 640, 480), 0, 0, 640, 480, [System.Drawing.GraphicsUnit]::Pixel)
        $comparison.Save((Join-Path $outputDir 'comparison.png'), [System.Drawing.Imaging.ImageFormat]::Png)
    } finally {
        $font.Dispose()
        $graphics.Dispose()
        $comparison.Dispose()
    }
} finally {
    $actual.Dispose()
    $expected.Dispose()
    $flipped.Dispose()
}
