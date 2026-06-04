$max_iters = 20
for ($i=0; $i -lt $max_iters; $i++) {
    Write-Host "Iteration $i"
    $output = pdflatex -interaction=nonstopmode main.tex 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Success!"
        break
    }
    # Look for missing .sty files
    if ($output -match "File ``([^']+)\.sty' not found") {
        $package = $matches[1]
        Write-Host "Missing package: $package"
        miktex packages install $package
    }
    elseif ($output -match "File ``([^']+)\.cls' not found") {
        $package = $matches[1]
        Write-Host "Missing package: $package"
        miktex packages install $package
    }
    else {
        Write-Host "Unknown error or done."
        break
    }
}
