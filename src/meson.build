sources = files(
    'main.c',
    'utils.c',
    'tokenizer.c',
    'parser.c',
    'builtins.c',
    'executor.c',
    'env.c'
)

flux_exe = executable('flux',
                      sources,
                      include_directories : inc,
                      install : true)

test('test', flux_exe)