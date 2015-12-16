(function($) {
    Drupal.behaviors.dmnbehaviors = {
        attach: function () {
            /*
                jQuery('#edit-dmn-fname').attr('placeholder','First Name');
                jQuery('#edit-dmn-lname').attr('placeholder','Last Name');;*/
            jQuery("#dmn-fieldset input[name=dmn_fname]").attr('placeholder','First Name');
            jQuery("#dmn-fieldset input[type=text]:eq(1)").attr('placeholder','Last Name');
        }
    };


    Drupal.ajax.prototype.commands.dmn_redirect_to_main = function(ajax, response, status){
        window.location = "/dmn";
    };


    Drupal.ajax.prototype.commands.dmn_rebuild = function(ajax, response, status){

            jQuery('#' + response.elementId).replaceWith('<div>hello, ' + response.fname + ' ' + response.lname + '</div>');

    };


    Drupal.ajax.prototype.commands.dmn_redirect = function(ajax, response, status){
        window.location = "/dmn/view2";
    };


    Drupal.ajax.prototype.commands.special_effects = function(ajax, response, status){
        jQuery('#' + response.elementId).hide().text(response.name).fadeIn(response.duration, function() {
            var fontSize = parseInt($(this).css('font-size'));
            jQuery(this).animate({'font-size': (response.amplify * fontSize) + "px"}, 5000, function() {
                jQuery(this).animate({'font-size': fontSize + "px"}, 5000);
            });
        });
    };
}(jQuery));







