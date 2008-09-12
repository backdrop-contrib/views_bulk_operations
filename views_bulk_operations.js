// $Id$

Drupal.vboSelectAll = function() {
  var table = this;

  thSelectAll = $('th.select-all', table).click(function() {
    cbSelectAll = $('input.form-checkbox', thSelectAll)[0];
    setSelectAll(false);
    $('input#vbo-select-all-pages', tdSelectAll).click(function() {
      setSelectAll(true);
    });
    $('input#vbo-select-this-page', tdSelectAll).click(function() {
      setSelectAll(false);
    });
    $('td input:checkbox', table).click(function() {
      setSelectAll($('form input#edit-objects-select-all').attr('value') == 1);
    });
  });

  function setSelectAll(all) {
    if (cbSelectAll.checked) {
      tdSelectAll = $('td.view-field-select-all').css('display', 'table-cell');
      if (all) {
        $('span#vbo-this-page', tdSelectAll).css('display', 'none');
        $('span#vbo-all-pages', tdSelectAll).css('display', 'inline');
        $('form input#edit-objects-select-all').attr('value', 1);
      }
      else {
        $('span#vbo-this-page', tdSelectAll).css('display', 'inline');
        $('span#vbo-all-pages', tdSelectAll).css('display', 'none');
        $('form input#edit-objects-select-all').attr('value', 0);
      }
    }
    else {
      tdSelectAll = $('td.view-field-select-all').css('display', 'none');
      $('form input#edit-objects-select-all').attr('value', all ? 1 : 0);
    }
  }
}

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('form table th.select-all').parents('table').each(Drupal.vboSelectAll);
  })
}
