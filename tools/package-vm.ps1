param(
    [string]$App = "calculator",
    [string]$Config = "Release",
    [string]$BuildRoot = "build",
    [string]$MesaRoot = "",
    [string]$OutputRoot = "dist\vm"
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$buildRootPath = if ([System.IO.Path]::IsPathRooted($BuildRoot)) {
    $BuildRoot
} else {
    Join-Path $repoRoot $BuildRoot
}
$buildDir = Join-Path $buildRootPath $Config
$exePath = Join-Path $buildDir "$App.exe"
$assetsPath = Join-Path $buildDir "assets"
$outputDir = Join-Path $repoRoot "$OutputRoot\$App"

if (-not (Test-Path $exePath)) {
    throw "Missing executable: $exePath. Build first with: cmake --build build --config $Config"
}

if (-not (Test-Path $assetsPath)) {
    throw "Missing assets directory: $assetsPath"
}

Remove-Item -LiteralPath $outputDir -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $outputDir | Out-Null

Copy-Item -LiteralPath $exePath -Destination $outputDir
Copy-Item -LiteralPath $assetsPath -Destination $outputDir -Recurse

if ($MesaRoot) {
    $resolvedMesaRoot = Resolve-Path $MesaRoot
    $mesaCandidates = @(
        (Join-Path $resolvedMesaRoot "x64"),
        (Join-Path $resolvedMesaRoot "mingw64"),
        $resolvedMesaRoot
    )

    $mesaBin = $null
    foreach ($candidate in $mesaCandidates) {
        if (Test-Path (Join-Path $candidate "opengl32.dll")) {
            $mesaBin = $candidate
            break
        }
    }

    if (-not $mesaBin) {
        throw "Could not find opengl32.dll under MesaRoot: $MesaRoot"
    }

    $mesaDlls = @(
        "opengl32.dll",
        "libglapi.dll",
        "libgallium_wgl.dll",
        "dxcompiler.dll",
        "dxil.dll"
    )

    foreach ($dll in $mesaDlls) {
        $source = Join-Path $mesaBin $dll
        if (Test-Path $source) {
            Copy-Item -LiteralPath $source -Destination $outputDir
        }
    }

    $launcher = @"
@echo off
set GALLIUM_DRIVER=llvmpipe
set MESA_LOADER_DRIVER_OVERRIDE=llvmpipe
set LIBGL_ALWAYS_SOFTWARE=1
start "" "%~dp0$App.exe"
"@
    Set-Content -LiteralPath (Join-Path $outputDir "run-$App-vm.bat") -Value $launcher -Encoding ASCII
} else {
    $readme = @"
This package does not include Mesa software OpenGL yet.

To make it run in a VM with no OpenGL driver:
1. Download a Windows x64 Mesa build.
2. Re-run:
   powershell -ExecutionPolicy Bypass -File tools\package-vm.ps1 -App $App -MesaRoot C:\path\to\mesa
3. Copy the generated dist\vm\$App folder to the VM and start run-$App-vm.bat.
"@
    Set-Content -LiteralPath (Join-Path $outputDir "README-VM.txt") -Value $readme -Encoding ASCII
}

Write-Host "VM package created: $outputDir"
