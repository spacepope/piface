{
	"targets": [
    		{
			"target_name": "pfio",
			'type': 'loadable_module',
			'product_extension': 'node',
			'include_dirs': [ '/usr/local/include' ],
			"sources": [ "pfio.cc" ],
			'link_settings': {
				'libraries': [
					'-lpifacedigital',
					'-lmcp23s17'
				]
			}
		}
	]
}
