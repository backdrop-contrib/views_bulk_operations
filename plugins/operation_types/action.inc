<?php

/**
 * @file
 * CTools plugin. Provides support for core actions.
 */

$plugin = array(
  'title' => t('Action'),
  'list callback' => 'views_bulk_operations_operation_action_list',
  'handler' => array(
    'file' => 'action.class.php',
    'class' => 'ViewsBulkOperationsAction',
  ),
);

function views_bulk_operations_operation_action_list() {
  $action_operations = actions_list();
  foreach ($action_operations as $callback => $operation) {
    $key = isset($operation['key']) ? $operation['key'] : $callback;
    $operations[$key] = array(
      'plugin' => 'action',
      'key' => $key,
      'label' => isset($operation['label']) ? $operation['label'] : '',
      'callback' => $callback,
      'parameters' => isset($operation['parameters']) ? $operation['parameters'] : array(),
      'configurable' => !empty($operation['configurable']),
      'type' => $operation['type'],
      'aggregate' => !empty($operation['aggregate']),
      'behavior' => isset($operation['behavior']) ? $operation['behavior'] : array(),
      'permissions' => isset($operation['permissions']) ? $operation['permissions'] : NULL,
      'pass rows' => !empty($operation['pass rows']),
    );
  }

  return $operations;
}
