sources = files('main.c', 'utils.c', 'command.c', 'tokenizer.c')

flux_exe = executable('flux',
                      sources,
                      include_directories : inc,
                      install : true)

test('test', flux_exe)