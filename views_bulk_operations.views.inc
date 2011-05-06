<?php

/**
 * Implements hook_views_data_alter().
 */
function views_bulk_operations_views_data_alter(&$data) {
  foreach (_views_bulk_operations_get_object_info() as $object_type => $object_info) {
    if (isset($data[$object_info['base_table']])) {
      $data[$object_info['base_table']]['views_bulk_operations'] = array(
        'title' => t('Bulk operations'),
        'help' => t('Execute bulk operations on the current object.'),
        'field' => array(
          'handler' => 'views_bulk_operations_handler_field_operations',
          'click sortable' => FALSE,
        ),
      );
    }
  }
}
