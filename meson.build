project('flux', 'c',
        version : '1.0.0',
        default_options : ['warning_level=3', 'c_std=c23'])

conf_data = configuration_data()
conf_data.set_quoted('MESON_VERSION_STR', meson.version())
conf_data.set_quoted('PROJECT_VERSION', meson.project_version())

subdir('include')

inc = include_directories('include')

subdir('src')