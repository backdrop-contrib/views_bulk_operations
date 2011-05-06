<?php

class views_bulk_operations_handler_field_operations extends views_handler_field {
  var $all_operations = array();

  function option_definition() {
    $options = parent::option_definition();

    $options['vbo'] = array(
      'contains' => array(
        'selected_operations' => array('default' => array()),
        'execution_type' => array('default' => VBO_EXECUTION_DIRECT),
        'display_type' => array('default' => 0),
        'skip_confirmation' => array('default' => FALSE),
        'display_result' => array('default' => TRUE),
        'merge_single_action' => array('default' => TRUE),
        'hide_select_all' => array('default' => FALSE),
      ),
    );

    // Per-action settings.
    $this->populate_operations();
    foreach ($this->all_operations as $key => $operation) {
      $form_function = $operation['callback'] . '_settings_form_options';
      if (function_exists($form_function)) {
        $options['vbo']['contains'][$key] = array('default' => call_user_func($form_function));
      }
    }

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['vbo'] = array(
      '#type' => 'fieldset',
      '#title' => t('Views Bulk Operations'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $execution = array(
      VBO_EXECUTION_DIRECT => t('Invoke them directly'),
      VBO_EXECUTION_BATCH => t('Use Batch API'),
    );
    if (module_exists('job_queue')) {
      $execution[VBO_EXECUTION_QUEUE] = t('Use <a href="@jobqueue">Job Queue</a>', array('@jobqueue' => url('http://drupal.org/project/job_queue')));
    }
    $form['vbo']['execution_type'] = array(
      '#type' => 'radios',
      '#title' => t('To execute operations'),
      '#default_value' => $this->options['vbo']['execution_type'],
      '#options' => $execution,
    );
    $form['vbo']['display_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display operations as'),
      '#default_value' => $this->options['vbo']['display_type'],
      '#options' => array(
        t('Dropdown selectbox with Submit button'),
        t('Each action as a separate button'),
      ),
    );
    $form['vbo']['hide_select_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide select all checkbox'),
      '#description' => t('Check this box to hide the "select all" checkbox and associated "select across all pages" button.'),
      '#default_value' => $this->options['vbo']['hide_select_all'],
    );
    $form['vbo']['skip_confirmation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Skip confirmation step'),
      '#description' => t('Check this box to skip the confirmation page on this view and directly execute the operation.'),
      '#default_value' => $this->options['vbo']['skip_confirmation'],
    );
    $form['vbo']['display_result'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display processing result'),
      '#description' => t('Check this box to let Drupal display a message with the result of processing the selected objects.'),
      '#default_value' => $this->options['vbo']['display_result'],
    );
    $form['vbo']['merge_single_action'] = array(
      '#type' => 'checkbox',
      '#title' => t('Merge single action\'s form with the row selection view'),
      '#description' => t('In case only one action is selected *and* this action is configurable, display its action form along with the row selection view.'),
      '#default_value' => $this->options['vbo']['merge_single_action'],
    );
    $form['vbo']['selected_operations'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Selected operations'),
      '#options' => $this->get_operations_options(),
      '#default_value' => $this->options['vbo']['selected_operations'],
    );

    // Per-action settings form.
    $object_info = _views_bulk_operations_object_info_for_view($this->view);
    foreach ($this->all_operations as $key => $operation) {
      if ($operation['type'] != 'system' && $operation['type'] != $object_info['type']) {
        continue;
      }

      $form_function = $operation['callback'] . '_settings_form';
      if (function_exists($form_function)) {
        $form['vbo'][$key] = array(
          '#tree' => TRUE,
          '#type' => 'fieldset',
          '#title' => $operation['label'],
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        $form['vbo'][$key] += call_user_func($form_function, $this->options['vbo'][$key]);
      }
    }
  }

  function options_validate(&$form, &$form_state) {
    foreach (array_filter($form_state['values']['vbo']['selected_operations']) as $selected_operation) {
      if (!isset($form_state['values']['vbo'][$selected_operation])) {
        continue;
      }

      $operation = $this->all_operations[$selected_operation];
      $form_function = $operation['callback'] . '_settings_form_validate';
      if (function_exists($form_function)) {
        $form_state['values']['vbo'][$selected_operation]['_error_element_base'] = 'vbo][' . $selected_operation . '][';
        call_user_func($form_function, $form, array('values' => $form_state['values']['vbo'][$selected_operation]));
      }
    }
  }

  function options_submit(&$form, &$form_state) {
    foreach (array_filter($form_state['values']['vbo']['selected_operations']) as $selected_operation) {
      if (!isset($form_state['values']['vbo'][$selected_operation])) {
        continue;
      }

      $operation = $this->all_operations[$selected_operation];
      $form_function = $operation['callback'] . '_settings_form_submit';
      if (function_exists($form_function)) {
        call_user_func($form_function, $form, array('values' => $form_state['values']['vbo'][$selected_operation]));
      }
    }
  }

  function query() {}

  /**
   * If the view is using a table style, provide a
   * placeholder for a "select all" checkbox.
   */
  function label() {
    if (is_a($this->view->style_plugin, 'views_plugin_style_table')) {
      //return '<!--views-form-select-all-->';
    }
    else {
      return parent::label();
    }
  }

  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Returns the form which replaces the placeholder from render().
   * This is done during post-processing by the views_form module.
   */
  function views_form($form, $form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return $form;
    }

    // If there's a session variable on this view, pre-load the old values.
    if (isset($_SESSION['vbo_values'][$_GET['q']])) {
      $session_objects = $_SESSION['vbo_values'][$_GET['q']]['objects'];
      $default_objects = array(
        'selection' => $session_objects['selection'], 'select_all' => $session_objects['select_all']
      );
    }
    else {
      $default_objects = array('selection' => array(), 'select_all' => FALSE);
    }

    $form[$this->options['id']] = array(
      '#tree' => TRUE,
    );
    // At this point, the query has already been run, so we can access the results
    // in order to get the base key value (for example, nid for nodes).
    foreach ($this->view->result as $row_id => $row) {
      $base_key_value = $row->{$this->view->base_field};

      if ($default_objects['select_all'] || !empty($default_objects['selection'][$base_key_value])) {
        $default_value = TRUE;
      }
      else {
        $default_value = FALSE;
      }

      $form[$this->options['id']][$row_id] = array(
        '#type' => 'checkbox',
        '#return_value' => $base_key_value,
        '#default_value' => $default_value,
      );
    }
    return $form;
  }

  function get_selected_operations() {
    $selected = array();
    foreach (array_filter($this->options['vbo']['selected_operations']) as $key) {
      if (!isset($this->all_operations[$key])) {
        continue;
      }
      if (module_exists('actions_permissions')) {
        $perm = actions_permissions_get_perm($this->all_operations[$key]['label'], $this->all_operations[$key]['callback']);
        if (!user_access($perm)) continue;
      }
      if (!empty($this->all_operations[$key]['permissions'])) foreach ($this->all_operations[$key]['permissions'] as $perm) {
        if (!user_access($perm)) {
          continue 2;
        }
      }
      $selected[$key] = $this->all_operations[$key]['label'];
    }
    return $selected;
  }

  function get_operation_info($key) {
    $operation = $this->all_operations[$key];
    if ($operation['type'] == 'system') { // System actions: morph them into current view type
      $object_info = _views_bulk_operations_object_info_for_view($this->view);
      $operation['type'] = $object_info['type'];
    }
    return $operation;
  }

  private function get_operations_options() {
    $options = array();
    $object_info = _views_bulk_operations_object_info_for_view($this->view);
    if (!$object_info) return NULL;
    foreach ($this->all_operations as $key => $operation) {
      if ($operation['type'] == 'system') { // System actions: morph them into current view type
        $operation['type'] = $object_info['type'];
      }
      if ($operation['type'] == $object_info['type']) {
        $options[$key] = $operation['label'] .' ('. $key .')';
      }
    }
    return $options;
  }

  private function populate_operations() {
    $operations = array();
    module_load_include('inc', 'node', 'node.admin');

    foreach (_views_bulk_operations_get_object_info() as $object_type => $object_info) {
      $hook_name = $object_type .'_operations';
      foreach (module_invoke_all($hook_name) as $operation) {
        if (empty($operation['callback'])) {
          continue;
        }

        $key = $operation['callback'] . (empty($operation['callback arguments']) ? '' : ':'. md5(serialize($operation['callback arguments'])));
        if (!isset($operation['behavior'])) { // assume operations modify nodes by default
          $operation['behavior'] = array('changes_node_property');
        }
        $operations[$key] = array(
          'key' => $key,
          'label' => $operation['label'],
          'callback' => $operation['callback'],
          'callback arguments' => isset($operation['callback arguments']) ? $operation['callback arguments'] : array(),
          'configurable' => isset($operation['configurable']) ? $operation['configurable'] : FALSE,
          'source' => 'operation',
          'type' => $object_type,
          'aggregate' => isset($operation['aggregate']) ? (int)$operation['aggregate'] : VBO_AGGREGATE_OPTIONAL,
          'access op' => $this->get_access_op($operation),
          'permissions' => isset($operation['permissions']) ? $operation['permissions'] : NULL,
        );
      }
    }

    $action_operations = actions_list() + $this->get_custom_actions();
    foreach ($action_operations as $callback => $operation) {
      $key = isset($operation['key']) ? $operation['key'] : $callback;
      $operations[$key] = array(
        'key' => $key,
        'label' => isset($operation['label']) ? $operation['label'] : '',
        'callback' => $callback,
        'callback arguments' => isset($operation['parameters']) ? $operation['parameters'] : array(),
        'configurable' => isset($operation['configurable']) ? $operation['configurable'] : FALSE,
        'source' => 'action',
        'type' => $operation['type'],
        'aggregate' => isset($operation['aggregate']) ? (int)$operation['aggregate'] : VBO_AGGREGATE_FORBIDDEN,
        'access op' => $this->get_access_op($operation),
        'permissions' => isset($operation['permissions']) ? $operation['permissions'] : NULL,
      );
    }

    uasort($operations, create_function('$a, $b', 'return strcasecmp($a["label"], $b["label"]);'));
    $this->all_operations = $operations;
  }

  private function get_access_op($operation) {
    $access_op = 0;
    if (isset($operation['behavior'])) {
      if (in_array('views_node_property', $operation['behavior'])) {
        $access_op |= VBO_ACCESS_OP_VIEW;
      }
      if (in_array('changes_property', $operation['behavior'])) {
        $access_op |= VBO_ACCESS_OP_UPDATE;
      }
      if (in_array('creates_node_property', $operation['behavior'])) {
        $access_op |= VBO_ACCESS_OP_CREATE;
      }
      if (in_array('deletes_node_property', $operation['behavior'])) {
        $access_op |= VBO_ACCESS_OP_DELETE;
      }
    }
    return $access_op;
  }

  private function get_custom_actions() {
    $actions = array();
    $static_actions = actions_list();
    $result = db_query("SELECT * FROM {actions} WHERE parameters > ''");
    foreach ($result as $action) {
      $parameters = unserialize($action->parameters);
      $actions[$action->aid] = array(
        'label' => isset($action->label) ? $action->label : '',
        'type' => $action->type,
        'configurable' => FALSE,
        'parameters' => $parameters,
        'key' => $action->callback . (empty($parameters) ? '' : ':'. md5($action->parameters)),
      );
      foreach (array('behavior', 'aggregate', 'permissions') as $attribute) {
        if (isset($static_actions[$action->callback][$attribute])) $actions[$action->aid][$attribute] = $static_actions[$action->callback][$attribute];
      }
    }
    return $actions;
  }

  function get_operation_settings($operation) {
    return (isset($this->options[$operation['key']]) ? $this->options[$operation['key']] : NULL);
  }
}
