<?php

function ld_component_features_export($data, &$export, $module_name){
	$export['dependencies']['ld_features'] = 'ld_features';
	foreach ($data as $component) {
		$export['features']['ld_component'][$component] = $component;
	}
	return array();
}

function ld_component_features_export_options() {
	module_load_include('inc', 'ld_features', 'ld_component');

	$options = array();
	foreach(ld_component_load() as $component) {
		$options[$component->id] = implode(',', $component->roles).': '.($component->destination_type ? 'PHP code' : $component->destination);
	}
	return $options;
}

function ld_component_features_export_render($module_name, $data, $export = NULL) {
	module_load_include('inc', 'ld_features', 'ld_component');

	$code = array();
	$code[] = '$ld_components = array();';
	foreach ($data as $name) {
		$code[] = "  \$ld_components['{$name}'] = " . features_var_export(ld_component_load($name)) .";";
	}
	$code[] = "return \$ld_components;";
	$code = implode("\n", $code);
	return array('ld_component_defaults' => $code);
}

function ld_component_features_revert($module_name) {
	ld_component_features_rebuild($module_name);
}

function ld_component_features_rebuild($module_name) {
	module_load_include('inc', 'ld_features', 'ld_component');

	$ld_components = module_invoke($module_name, 'ld_component_defaults');
	if (!empty($ld_components)) {
		foreach ($ld_components as $ld_component) {
			ld_component_save($ld_component);
		}
	}
}