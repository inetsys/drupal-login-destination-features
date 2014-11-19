<?php

function ld_component_load($id=null){
	$roles = user_roles();
	$query = db_select('login_destination','ld')
		->fields('ld');
	if($id){
		$query->condition('id', $id, '=');
	}
	$query->orderBy('id', 'asc');
	$result = $query->execute();

	$list = array();
	foreach($result as $record){
		// Roles
		$record->roles = unserialize($record->roles);
		$clean_roles = array();
		foreach($roles as $key => $name){
			if(isset($record->roles[$key])){
				$clean_roles[$name] = $name;
			}
		}
		$record->roles = $clean_roles;
		// Triggers
		$record->triggers = unserialize($record->triggers);
		$list[] = $record;
	}

	return $id && count($list) == 1 ? $list[0] : $list;
}

function ld_component_delete($ld_component){
	$ld_component = (object) $ld_component;

	db_delete('login_destination')
		->condition('id', $ld_component->id)
		->execute();
}

function ld_component_save($ld_component){
	$roles = user_roles();
	$rids = array();

	$ld_component = (object) $ld_component;

	foreach($ld_component->roles as $ld_name){
		foreach($roles as $rid => $name){
			if($ld_name == $name){
				$rids[$rid] = $rid;
				break;
			}
		}
	}

	ld_component_delete($ld_component);
	db_insert('login_destination')
		->fields(array(
			'id' => $ld_component->id,
			'triggers' => serialize($ld_component->triggers),
			'roles' => serialize($rids),
			'pages_type' => $ld_component->pages_type,
			'pages' => $ld_component->pages,
			'destination_type' => $ld_component->destination_type,
			'destination' => $ld_component->destination,
			'weight' => $ld_component->weight,
		))
		->execute();
}