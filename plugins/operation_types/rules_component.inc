<?php

/**
 * @file
 * CTools plugin. Provides support for rules components (rule, ruleset, action).
 */

$plugin = array(
  'title' => t('Rules component'),
  'list callback' => 'views_bulk_operations_operation_rules_component_list',
  'handler' => array(
    'file' => 'rules_component.class.php',
    'class' => 'ViewsBulkOperationsRulesComponent',
  ),
);

function views_bulk_operations_operation_rules_component_list() {
  if (!module_exists('rules')) {
    return;
  }

  $entity_info = entity_get_info();
  $entity_types = array_keys($entity_info);
  $supported_types = array('entity');
  foreach ($entity_types as $type) {
    $supported_types[] = $type;
  }
  foreach (rules_get_components(FALSE, 'action') as $component_key => $component) {
    $parameter_info = $component->parameterInfo();
    $first_parameter = reset($parameter_info);
    $parameter_keys = array_keys($parameter_info);
    $entity_key = reset($parameter_keys);
    // If the first param is not an entity type, skip the component.
    if (!in_array($first_parameter['type'], $supported_types)) {
      continue;
    }

    $operations[$component_key] = array(
      'plugin' => 'rules_component',
      'key' => $component_key,
      'label' => $component->label,
      'parameters' => array('component_key' => $component_key, 'entity_key' => $entity_key),
      'configurable' => count($parameter_info) > 1,
      'type' => $first_parameter['type'],
    );
  }

  return $operations;
}
