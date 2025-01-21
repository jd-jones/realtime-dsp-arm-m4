" 'cc' -> ['gcc', 'clang']
" ['cc', 'ccls', 'clangd', 'clangtidy', 'cppcheck', 'cquery', 'flawfinder']
let g:ale_linters = {'c': ['cc']}
let g:ale_c_cc_executable = 'arm-none-eabi-gcc'
let g:ale_c_cc_options = '-std=c99 -Wall'
